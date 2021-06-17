
#include <iostream>
#include <sstream>
#include <numeric>
#include <fstream>


#include "ArgumentParser.h"

#include "common.h"

#include "export.h"
#include "CSVImporter.h"
#include "Tree.h"
#include "AttributeSumAggregation.h"
#include "TreeProcessing.h"
#include "TreemapLayouter.h"
#include "Rect.h"
#include "CPUMeasurement.h"


namespace
{


void streamUsage(std::ostream & stream)
{
    stream.flush();
}


inline std::size_t getIndex(unsigned int x, unsigned int y, std::size_t width)
{
    return y * width + x;
}

inline std::pair<unsigned int, unsigned int> getComponents(std::size_t index, std::size_t width)
{
    return std::make_pair(index % width, index / width);
}


std::pair<float,float> compareMatrices(std::vector<float> & matrix, std::vector<float> & matrixComp, std::size_t sharedNodeCount)
{
    if (matrix.size() <= 1)
    {
        return std::make_pair(0.0f, 0.0f);
    }

    std::vector<float> rowDeltas(sharedNodeCount-1);

    auto squaredDeltaSum = 0.0f;
    auto squaredDeltaSumForAAD = 0.0f;
    for (size_t i = 0; i < sharedNodeCount; ++i)
    {
        auto sum = 0.0f;

        auto indexOffset = 0;
        for (size_t j = 0; j < sharedNodeCount; ++j)
        {
            if (i == j)
            {
                indexOffset = -1;

                continue;
            }
            else
            {
                const auto index = getIndex(i, j, sharedNodeCount);

                const auto element = matrix.at(index);
                const auto elementComp = matrixComp.at(index);

                auto angleDelta = element - elementComp;
                while (angleDelta > glm::pi<float>())
                {
                    angleDelta -= 2 * glm::pi<float>();
                }
                while (angleDelta < - glm::pi<float>())
                {
                    angleDelta += 2 * glm::pi<float>();
                }

                rowDeltas[j + indexOffset] = angleDelta;
                sum += angleDelta;
            }
        }

        auto average = sum / rowDeltas.size();
        for (size_t j = 0; j < rowDeltas.size(); ++j)
        {
            squaredDeltaSum += glm::pow(rowDeltas[j] - average, 2.0f);
            squaredDeltaSumForAAD += glm::pow(rowDeltas[j], 2.0f);
        }
    }

    return std::make_pair(glm::pow(squaredDeltaSum / (sharedNodeCount * (sharedNodeCount-1)), 0.5f),
                          glm::pow(squaredDeltaSumForAAD / (sharedNodeCount * (sharedNodeCount-1)), 0.5f));
}

void fillMatrix(std::vector<float> & matrix, const Tree *tree, const std::unordered_map<std::uint32_t, std::uint32_t> & nodeIndexById, const Buffer<Rect> &layout, const std::vector<std::uint32_t> & occuringInBothRevisions)
{
    for (auto index = size_t(0); index < matrix.size(); ++index)
    {
        auto x = 0u;
        auto y = 0u;

        std::tie(x, y) = getComponents(index, occuringInBothRevisions.size());

        if (x == y)
        {
            matrix[index] = 0.0f;

            return;
        }

        const auto a = layout[tree->at(nodeIndexById.at(occuringInBothRevisions[x]))].center();
        const auto b = layout[tree->at(nodeIndexById.at(occuringInBothRevisions[y]))].center();

        if (glm::dot(a, b) <= glm::epsilon<float>())
        {
            matrix[index] = 0.0f;
        }
        else
        {
            matrix[index] = glm::atan(b.y - a.y, b.x - a.x);
        }
    };
}


float computeAverageAspectRatio(Tree * tree, const Buffer<Rect> & layout)
{
    auto summedAspectRatio = 0.0f;
    auto numberOfNodes = 0;
    tree->nodesDo([& layout, & summedAspectRatio, & numberOfNodes](const TreeNode * node) {
        const auto & rect = layout.at(node->index());

        if (rect.area() >= glm::epsilon<float>())
        {
            summedAspectRatio += rect.aspectRatio();
            numberOfNodes += 1;
        }
    });

    return numberOfNodes == 0 ? 0.0f : summedAspectRatio / numberOfNodes;
}

float computeAverageAspectRatioChange(Tree * tree, const std::unordered_map<std::uint32_t, std::uint32_t> & nodeIndexById, const Buffer<Rect> & layout, Tree * treeComp, const std::unordered_map<std::uint32_t, std::uint32_t> & nodeIndexByIdComp, const Buffer<Rect> & layoutComp, const std::vector<std::uint32_t> & occuringInBothRevisions)
{
    auto numberOfNodes = occuringInBothRevisions.size();

    if (numberOfNodes < 1)
    {
        return 0.0f;
    }

    auto summedAspectRatioChange = 0.0f;

    for (const auto & id : occuringInBothRevisions)
    {
        const auto & rect = layout.at(tree->at(nodeIndexById.at(id)));
        const auto & rectComp = layoutComp.at(treeComp->at(nodeIndexByIdComp.at(id)));

        const auto ar = rect.aspectRatio();
        const auto arComp = rectComp.aspectRatio();

        summedAspectRatioChange += glm::max(ar / arComp, arComp / ar);
    }

    return summedAspectRatioChange / numberOfNodes;
}

float computeAverageDistanceChange(Tree * tree, const std::unordered_map<std::uint32_t, std::uint32_t> & nodeIndexById, const Buffer<Rect> & layout, Tree * treeComp, const std::unordered_map<std::uint32_t, std::uint32_t> & nodeIndexByIdComp, const Buffer<Rect> & layoutComp, const std::vector<std::uint32_t> & occuringInBothRevisions)
{
    auto numberOfNodes = occuringInBothRevisions.size();

    if (numberOfNodes < 1)
    {
        return 0.0f;
    }

    auto summedDistanceChange = 0.0f;

    for (const auto & id : occuringInBothRevisions)
    {
        const auto & rect = layout.at(tree->at(nodeIndexById.at(id)));
        const auto & rectComp = layoutComp.at(treeComp->at(nodeIndexByIdComp.at(id)));

        summedDistanceChange += glm::distance(rect.center(), rectComp.center());
    }

    return summedDistanceChange / numberOfNodes;
}

Rect getSection(const Rect & r1, size_t i)
{
    // divide the space around r1 into 8 sections s1 to s8
    //              left,             bottom,             width,      height
    switch (i) {
    case 0:
        return Rect(r1.left() - 1.0f, r1.bottom(),        1.0f,       r1.height()); // left
    case 1:
        return Rect(r1.left(),        r1.top(),           r1.width(), 1.0f);        // top
    case 2:
        return Rect(r1.right(),       r1.bottom(),        1.0f,       r1.height()); // right
    case 3:
        return Rect(r1.left(),        r1.bottom() - 1.0f, r1.width(), 1.0f);        // bottom
    case 4:
        return Rect(r1.left() - 1.0f, r1.top(),           1.0f,       1.0f);        // top-left
    case 5:
        return Rect(r1.right(),       r1.top(),           1.0f,       1.0f);        // top-right
    case 6:
        return Rect(r1.right(),       r1.bottom() - 1.0f, 1.0f,       1.0f);        // bottom-right
    default:
    case 7:
        return Rect(r1.left() - 1.0f, r1.bottom() - 1.0f, 1.0f,       1.0f);        // bottom-left
    }
}

float computeRelativePositionChange(
        Tree * tree,
        const std::unordered_map<std::uint32_t, std::uint32_t> & nodeIndexById,
        const Buffer<std::uint32_t> & nodeIdByIndex,
        const Buffer<Rect> & layout,
        Tree * treeComp,
        const std::unordered_map<std::uint32_t, std::uint32_t> & nodeIndexByIdComp,
        const Buffer<std::uint32_t> & nodeIdByIndexComp,
        const Buffer<Rect> & layoutComp,
        const std::vector<std::uint32_t> & occuringInBothRevisions)
{
    // implemented according to "Stable Treemaps via Local Moves" Sondag et al.

    auto numberOfNodes = occuringInBothRevisions.size();

    if (numberOfNodes < 1)
    {
        return 0.0f;
    }

    auto summedPositionChange = 0.0f;

    for (const auto & id1 : occuringInBothRevisions)
    {
        for (const auto & id2 : occuringInBothRevisions)
        {
            if (tree->at(nodeIndexById.at(id1))->parent() != tree->at(nodeIndexById.at(id2))->parent())
            {
                // do not compare rectangles from different parents
                continue;
            }
            const auto & rect1 = layout.at(tree->at(nodeIndexById.at(id1)));
            const auto & rectComp1 = layoutComp.at(treeComp->at(nodeIndexByIdComp.at(id1)));

            const auto & rect2 = layout.at(tree->at(nodeIndexById.at(id2)));
            const auto & rectComp2 = layoutComp.at(treeComp->at(nodeIndexByIdComp.at(id2)));

            float change = 0.0f;
            for (size_t i = 0; i < 8; i++)
            {
                const auto intersection = getSection(rect1, i).intersection(rect2);
                const auto intersectionComp = getSection(rectComp1, i).intersection(rectComp2);
                const auto newChange = std::abs(intersection.area() / rect2.area() - intersectionComp.area() / rectComp2.area());
                change += newChange;
            }
            summedPositionChange += change / 2;
        }
    }

    return summedPositionChange / (numberOfNodes * numberOfNodes);
}

/*
float computeRelativeParentChange(Tree * tree, const std::map<std::uint32_t, std::uint32_t> & nodeIndexById, const Buffer<Rect> & layout, Tree * treeComp, const std::map<std::uint32_t, std::uint32_t> & nodeIndexByIdComp, const Buffer<Rect> & layoutComp, const std::vector<std::uint32_t> & occuringInBothRevisions)
{
    auto numberOfNodes = occuringInBothRevisions.size();

    if (numberOfNodes < 2) // At least one leaf + root required
    {
        return 0.0f;
    }

    auto summedRelativeParentChange = 0.0f;

    for (const auto & id : occuringInBothRevisions)
    {
        if (id == 0)
        {
            --numberOfNodes;

            continue;
        }

        const auto node = tree->at(nodeIndexById.at(id));
        const auto parent = tree->at(node->parent());
        const auto nodeComp = treeComp->at(nodeIndexByIdComp.at(id));
        const auto parentComp = treeComp->at(nodeComp->parent());

        const auto & rect = layout.at(node);
        const auto & parentRect = layout.at(parent);
        const auto & rectComp = layoutComp.at(nodeComp);
        const auto & parentRectComp = layoutComp.at(parentComp);

        // Compute relative parent distance (rpd)
        const auto rpd = glm::distance(parentRect.center(), rect.center()) / (glm::length(parentRect.extent) / 2.0f);
        const auto rpdComp = glm::distance(parentRectComp.center(), rectComp.center()) / (glm::length(parentRectComp.extent) / 2.0f);

        summedRelativeParentChange += glm::abs(rpd - rpdComp);
    }

    return summedRelativeParentChange / numberOfNodes;
}
*/

std::pair<float,float> computeAngularMetrics(Tree * tree, const std::unordered_map<std::uint32_t, std::uint32_t> & nodeIndexById, const Buffer<Rect> & layout, Tree * treeComp, const std::unordered_map<std::uint32_t, std::uint32_t> & nodeIndexByIdComp, const Buffer<Rect> & layoutComp, const std::vector<std::uint32_t> & occuringInBothRevisions)
{
    const auto sharedNodeCount = occuringInBothRevisions.size();

    if (sharedNodeCount <= 1)
    {
        return std::make_pair(0.0f, 0.0f);
    }

    // Initialize angularOrientations
    std::vector<float> angularOrientations(sharedNodeCount * sharedNodeCount);
    std::vector<float> angularOrientationsComp(sharedNodeCount * sharedNodeCount);

    fillMatrix(angularOrientations, tree, nodeIndexById, layout, occuringInBothRevisions);
    fillMatrix(angularOrientationsComp, treeComp, nodeIndexByIdComp, layoutComp, occuringInBothRevisions);

    float rdc = 0;
    float aad = 0;

    std::tie(rdc, aad) = compareMatrices(angularOrientations, angularOrientationsComp, sharedNodeCount);

    rdc /= glm::pi<float>();
    aad /= glm::pi<float>();

    return std::make_pair(rdc, aad);
}

float computeLocationDrift(const std::vector<Tree *> & trees, std::uint32_t maxId, const std::vector<Buffer<std::uint32_t>> & index2ids, const std::vector<std::unordered_map<std::uint32_t, std::uint32_t>> & nodeIndexByIds, const std::vector<Buffer<Rect>> & layouts)
{
    std::vector<glm::vec2> mergedCenters(maxId+1);
    std::vector<unsigned int> occurrencesPerNode(maxId+1);

    for (std::uint32_t i = 0; i < trees.size(); ++i)
    {
        const auto tree = trees[i];
        const auto & ids = index2ids[i];

        for (auto index = std::uint32_t(0); index < tree->size(); ++index)
        {
            const auto node = tree->at(index);

            assert(node != nullptr);

            if (layouts.at(i).at(index).area() >= glm::epsilon<float>())
            {
                mergedCenters.at(ids.at(node)) += layouts.at(i).at(index).center();
                occurrencesPerNode.at(ids.at(node)) += 1;
            }
        };
    }

    for (auto index = std::size_t(0); index < mergedCenters.size(); ++index)
    {
        auto & center = mergedCenters.at(index);
        const auto occurrence = occurrencesPerNode.at(index);

        assert(!std::isnan(center.x));
        assert(!std::isnan(center.y));

        if (occurrence > 0)
        {
            center /= float(occurrence);
        }

        assert(!std::isnan(center.x));
        assert(!std::isnan(center.y));
    }

    std::vector<float> locationDrifts(maxId+1);

    for (std::uint32_t i = 0; i < trees.size(); ++i)
    {
        const auto tree = trees.at(i);
        const auto & ids = index2ids[i];

        for (auto index = std::uint32_t(0); index < tree->size(); ++index)
        {
            const auto node = tree->at(index);

            const auto & center = mergedCenters.at(ids.at(node));

            if (layouts.at(i).at(index).area() >= glm::epsilon<float>())
            {
                locationDrifts.at(ids.at(node)) += glm::distance(center, layouts.at(i).at(index).center());
            }
        }
    }

    for (auto index = std::size_t(0); index < locationDrifts.size(); ++index)
    {
        auto & locationDrift = locationDrifts.at(index);
        const auto occurrence = occurrencesPerNode.at(index);

        if (occurrence > 0)
        {
            locationDrift /= float(occurrence);
        }
    }

    auto summedLocationDrift = 0.0f;
    auto numberOfNodes = 0;

    for (auto i = size_t(0); i < locationDrifts.size(); ++i)
    {
        if (occurrencesPerNode.at(i) > 0)
        {
            summedLocationDrift += locationDrifts.at(i);

            assert(!std::isnan(summedLocationDrift));

            numberOfNodes += 1;
        }
    }

    return numberOfNodes >= 1 ? summedLocationDrift / numberOfNodes : 0.0f;
}


} // namespace


int main(int argc, char ** argv)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    std::clog.tie(nullptr);
    std::cerr.tie(nullptr);

    auto arguments = ArgumentParser();
    arguments.parse(argc, argv);

    const auto multipleDatasets = arguments.isSet("--");

    auto filename = arguments.value<std::string>("--dataset", "");
    auto filenames = arguments.value<std::string>("--datasets", "");
    auto filenamesPrefix = arguments.value<std::string>("--datasets-prefix", "");
    auto weightAttribute = arguments.value<std::string>("--weights", "");
    auto partitionAlg = arguments.value<std::string>("--partition", "greedy");
    auto layoutAlg = arguments.value<std::string>("--algorithm", "hilbert");

    if (weightAttribute == "" || (!multipleDatasets && filename == "" && filenames == ""))
    {
        std::cerr << "No dataset found or no weights attribute passed" << std::endl;
        return 1;
    }

    const auto measurePerformance = arguments.isSet("-pm");
    const auto measureLayoutMetrics = arguments.isSet("-lm");
    // const auto flatImport = arguments.isSet("-f");
    const auto exportSVG = arguments.isSet("-es");
    const auto exportLayout = arguments.isSet("-el");
    const auto printStatistics = arguments.isSet("-p");

    streamUsage(std::cerr);

    auto importer = CSVImporter();
    auto treeProcessing = TreeProcessing();
    auto aggregation = AttributeSumAggregation();
    auto layouter = TreemapLayouter();

    if (layoutAlg == "hilbert" && partitionAlg == "greedy") {
        layouter.setAlgorithm(RectangularLayoutAlgorithm::Hilbert_Greedy);
    } else if (layoutAlg == "hilbert" && partitionAlg == "minmax") {
        layouter.setAlgorithm(RectangularLayoutAlgorithm::Hilbert_MinMax);
    } else if (layoutAlg == "hilbert" && partitionAlg == "minvariance") {
        layouter.setAlgorithm(RectangularLayoutAlgorithm::Hilbert_MinVariance);
    } else if (layoutAlg == "moore" && partitionAlg == "greedy") {
        layouter.setAlgorithm(RectangularLayoutAlgorithm::Moore_Greedy);
    } else if (layoutAlg == "moore" && partitionAlg == "minmax") {
        layouter.setAlgorithm(RectangularLayoutAlgorithm::Moore_MinMax);
    } else if (layoutAlg == "moore" && partitionAlg == "minvariance") {
        layouter.setAlgorithm(RectangularLayoutAlgorithm::Moore_MinVariance);
    } else {
        std::cerr << "Unknown curve type or partition algorithm: " << layoutAlg << "/" << partitionAlg << std::endl;
    }

    std::vector<Tree *> trees;
    std::vector<Buffer<std::uint32_t>> index2id; // tree index to global id
    std::vector<std::unordered_map<std::uint32_t, std::uint32_t>> id2index; // global id to tree index
    std::unordered_map<std::string, std::uint32_t> path2id; // path to global id

    auto mapToGlobalIds = [&path2id](const std::vector<std::string>& paths) {
        Buffer<std::uint32_t> res(std::vector<std::uint32_t>(paths.size()));

        for (int index = 0; index < paths.size(); ++index)
        {
            auto it = path2id.find(paths[index]);
            if(it == end(path2id))
            {
                it = path2id.emplace(paths[index], path2id.size()).first;
            }

            res[index] = it->second;
        }

        return res;
    };

    if (filenames != "")
    {
        std::string f;
        std::ifstream fs(filenames);
        while (fs.good() && std::getline(fs, f))
        {
            Tree * tree = nullptr;

            std::vector<std::string> paths;
            tree = treeProcessing.removeZeroWeightNodesAndDeleteSource(importer.load(filenamesPrefix + f, weightAttribute, &paths), weightAttribute, &paths);

            if (tree == nullptr) // ignore empty trees
            {
                continue;
            }

            auto & currentIndex2id = index2id.emplace_back(mapToGlobalIds(paths));
            auto & currentID2index = id2index.emplace_back(std::unordered_map<std::uint32_t, std::uint32_t>({}));

            for (auto i = 0; i < currentIndex2id.size(); ++i)
            {
                currentID2index[currentIndex2id[i]] = i;
            }

            trees.push_back(tree);
        }

        // TODO sort later trees by global_id ?
    }

    if (multipleDatasets)
    {
        std::string f;
        while (std::getline(std::cin, f))
        {
            Tree * tree = nullptr;

            std::vector<std::string> paths;
            tree = treeProcessing.removeZeroWeightNodesAndDeleteSource(importer.load(f, weightAttribute, &paths), weightAttribute, &paths);

            if (tree == nullptr) // ignore empty trees
            {
                continue;
            }

            auto & currentIndex2id = index2id.emplace_back(mapToGlobalIds(paths));
            auto & currentID2index = id2index.emplace_back(std::unordered_map<std::uint32_t, std::uint32_t>({}));

            for (auto i = 0; i < currentIndex2id.size(); ++i)
            {
                currentID2index[currentIndex2id[i]] = i;
            }

            trees.push_back(tree);
        }

        // TODO sort later trees by global_id ?
    }

    if (filename != "")
    {
        Tree * tree = nullptr;

        std::vector<std::string> paths;
        tree = treeProcessing.removeZeroWeightNodesAndDeleteSource(importer.load(filename, weightAttribute, &paths), weightAttribute, &paths);
        index2id.emplace_back(mapToGlobalIds(paths));

        trees.push_back(tree);
    }

    /*if (flatImport)
    {
        for (auto & tree : trees)
        {
            tree = flatten(tree, weightAttribute);
        }
    }*/

    if (printStatistics)
    {
        std::cerr << "Dataset " << filename << (multipleDatasets ? " (multiple)" : "") << " weights attribute " << weightAttribute << std::endl;
    }

    std::vector<Buffer<Rect>> layouts;

    Buffer<float> * weightValues = nullptr; // last weight values
    
    auto i = 0;
    for (const auto & tree : trees)
    {
        ++i;
        weightValues = &tree->attributeValues(weightAttribute);

        if (printStatistics)
        {
            tree->outputStatistics(std::cout);
        }

        aggregation.process(tree, *weightValues);

        Buffer<Rect> layout(std::vector<Rect>(tree->size()));

        if (measurePerformance)
        {
            auto duration = 0.0f;
            auto durationPerIteration = 0.0f;

            std::tie(duration, durationPerIteration) = CPUMeasurement("Treemap Layouting " + std::to_string(i)).measure([&]() {
                layouter.process(
                    tree,
                    *weightValues,
                    layout
                );
            }, 500, 500);

            std::cout << layout.size()
                << ";" << durationPerIteration
                << ";" << duration
                << std::endl;
        }
        else
        {
            layouter.process(
                tree,
                *weightValues,
                layout
            );
        }

        layouts.push_back(layout);
    }

    // Layout Measurements
    if (measureLayoutMetrics)
    {
        // std::cout << "Layout,AAR,AARC,ADC,RPC,RDC,LD" << std::endl;
        std::cout << "Layout,AAR,ADC,RDC,LD,RPC,AAD" << std::endl;

        auto summedAverageAspectRatio = 0.0f;
        // auto summedAverageAspectRatioChange = 0.0f;
        auto summedAverageDistanceChange = 0.0f;
        // auto summedRelativeParentChange = 0.0f;
        auto summedRelativeDirectionChange = 0.0f;
        auto summedRelativePositionChange = 0.0f;
        auto summedAverageAngularDisplacement = 0.0f;

        for (auto i = std::uint32_t(0); i < trees.size(); ++i)
        {
            std::vector<std::uint32_t> occuringInBothRevisions;

            const auto currentTree = trees[i];
            // const auto & currentNodeIndicesById = id2index[i];
            const auto & currentIDs = index2id[i];

            if (i > 0)
            {
                const auto prevTree = trees[i-1];
                const auto & prevNodeIndicesById = id2index[i-1];
                // const auto & prevIDs = index2id[i-1];

                for (size_t j = 0; j < currentTree->size(); j++)
                {
                    const auto node = currentTree->at(j);

                    const auto currentId = currentIDs.at(node);

                    const auto & layoutElement = layouts[i][node];

                    if (layoutElement.area() >= glm::epsilon<float>())
                    {
                        const auto it = prevNodeIndicesById.find(currentId);

                        if (it != prevNodeIndicesById.end())
                        {
                            const auto compNode = prevTree->at(it->second);

                            if (compNode != nullptr)
                            {
                                const auto & layoutElementComp = layouts[i-1][compNode];

                                if (layoutElementComp.area() >= glm::epsilon<float>())
                                {
                                    occuringInBothRevisions.push_back(currentId);
                                }
                            }
                        }
                    }
                }
            }

            const auto averageAspectRatio = computeAverageAspectRatio(trees[i], layouts[i]);
            //const auto averageAspectRatioChange = i > 0 ? computeAverageAspectRatioChange(trees[i-1], id2index[i-1], layouts[i-1], trees[i], id2index[i], layouts[i], occuringInBothRevisions) : -1.0f;
            const auto averageDistanceChange = i > 0 ? computeAverageDistanceChange(trees[i-1], id2index[i-1], layouts[i-1], trees[i], id2index[i], layouts[i], occuringInBothRevisions) : -1.0f;
            // const auto relativeParentChange = -1.0f; // i > 0 ? computeRelativeParentChange(trees[i-1], id2index[i-1], layouts[i-1], trees[i], id2index[i], layouts[i], occuringInBothRevisions) : -1.0f;
            float relativeDirectionChange = -1.0f;
            float averageAngularDisplacement = -1.0f;
            if (i > 0)
            {
                std::tie(relativeDirectionChange, averageAngularDisplacement) = computeAngularMetrics(trees[i-1], id2index[i-1], layouts[i-1], trees[i], id2index[i], layouts[i], occuringInBothRevisions);
            }
            const auto locationDrift = -1.0f;
            const auto relativePositionChange = i > 0 ? computeRelativePositionChange(trees[i-1], id2index[i-1], index2id[i-1], layouts[i-1], trees[i], id2index[i], index2id[i], layouts[i], occuringInBothRevisions) : -1.0f;

            std::cout << i
                << "," << averageAspectRatio
                // << "," << averageAspectRatioChange
                << "," << averageDistanceChange
                // << "," << relativeParentChange
                << "," << relativeDirectionChange
                << "," << locationDrift
                << "," << relativePositionChange
                << "," << averageAngularDisplacement
                << std::endl;

            summedAverageAspectRatio += averageAspectRatio;
            // summedAverageAspectRatioChange += averageAspectRatioChange;
            summedAverageDistanceChange += averageDistanceChange;
            // summedRelativeParentChange += relativeParentChange;
            summedRelativeDirectionChange += relativeDirectionChange;
            summedRelativePositionChange += relativePositionChange;
            summedAverageAngularDisplacement += averageAngularDisplacement;
        }

        const auto maxId = path2id.size();
        const auto locationDrift = computeLocationDrift(trees, maxId, index2id, id2index, layouts);

        const auto averageAspectRatio = summedAverageAspectRatio / trees.size();
        // const auto averageAspectRatioChange = trees.size() <= 1 ? 0.0f : summedAverageAspectRatioChange / (trees.size()-1);
        const auto averageDistanceChange = trees.size() <= 1 ? 0.0f : summedAverageDistanceChange / (trees.size()-1);
        // const auto relativeParentChange = trees.size() <= 1 ? 0.0f : summedRelativeParentChange / (trees.size()-1);
        const auto relativeDirectionChange = trees.size() <= 1 ? 0.0f : summedRelativeDirectionChange / (trees.size()-1);
        const auto relativePositionChange = trees.size() <= 1 ? 0.0f : summedRelativePositionChange / (trees.size()-1);
        const auto averageAngularDisplacement = trees.size() <= 1 ? 0.0f : summedAverageAngularDisplacement / (trees.size()-1);

        std::cout << "Sum"
            << "," << averageAspectRatio
            // << "," << averageAspectRatioChange
            << "," << averageDistanceChange
            // << "," << relativeParentChange
            << "," << relativeDirectionChange
            << "," << locationDrift
            << "," << relativePositionChange
            << "," << averageAngularDisplacement
            << std::endl;
    }

    // Continue with last layout and tree

    const auto tree = trees.back();
    const auto & layout = layouts.back();

    if (exportSVG)
    {
        writeToSVG(layout, tree, 1000.0f, false, SVGColorType::GrayScaleContinuous, std::cout);
    }
    if (exportLayout)
    {
        writeToFile(layout, std::cout);
    }

    for (const auto & tree : trees)
    {
        delete tree;
    }

    return 0;
}
