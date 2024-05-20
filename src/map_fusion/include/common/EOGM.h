#pragma once

#include <vector>
#include <map>
#include "BeliefMassFunction.h"
#include <nav_msgs/OccupancyGrid.h>
#include <tuple>
#include <octomap/ColorOcTree.h>

class EOGM
{
public:
    // - Methods
    // - - Constructors
    EOGM() = delete;
    EOGM(float resolution);
    EOGM(unsigned int width, unsigned int height, float resolution);
    EOGM(std::vector<std::vector<float>> &occupied, std::vector<std::vector<float>> &free, float resolution);
    EOGM(nav_msgs::OccupancyGrid occupancy_grid, nav_msgs::OccupancyGrid free_grid, float resolution);
    // - - Destructor
    ~EOGM();

    // - - Getters
    /// @brief Get the occupancy grid
    /// @return nav_msgs::OccupancyGrid
    nav_msgs::OccupancyGrid getOccupancyGrid();
    nav_msgs::OccupancyGrid getFreeGrid();
    octomap::ColorOcTree &getOctomap();

    unsigned int getWidth() const;
    unsigned int getHeight() const;
    float getRealWidth() const;
    float getRealHeight() const;

    // - - Setters
    void setOrigin(double x, double y);

    // - - Operations

    void setCell(int x, int y, BeliefMassFunction value);
    void resize(unsigned int width, unsigned int height);

    ///@brief Fuse this EOGM with another using Dempster-Shafer theory. This also update the associated octomap.
    ///
    ///@param other  The other EOGM to fuse with
    void fuse(const EOGM &other);

    // - - Conversion
    inline static int8_t fromFloatToByte(float value)
    {
        return static_cast<int8_t>(std::round((value * 0xFF) + INT8_MIN));
    }

    inline static float fromByteToFloat(int8_t value)
    {
        return (static_cast<float>(value) - INT8_MIN) / 0xFF;
    }

    // - - Operators
    ///@brief Add (fuse) two EOGMs together
    ///
    ///@param other
    EOGM &operator+=(const EOGM &other);

private:
    // - Methods
    void updateOctomap(int x, int y, const BeliefMassFunction *);
    void updateOctomap(octomap::point3d[], const BeliefMassFunction *[], size_t size);

    // - Attributes
    // - - Constants
    const octomap::ColorOcTreeNode::Color occupied_color = octomap::ColorOcTreeNode::Color(255, 0, 0);
    const octomap::ColorOcTreeNode::Color free_color = octomap::ColorOcTreeNode::Color(0, 255, 0);
    const octomap::ColorOcTreeNode::Color conflict_color = octomap::ColorOcTreeNode::Color(0, 0, 255);

    // - - Variables
    std::vector<std::vector<BeliefMassFunction>> grid;

    double x, y;
    float resolution;
    octomap::ColorOcTree *tree;
};