/**
  * ArUco gridboard detector, following the same pattern as ModifiedCircleGridObservationFinder.
  * Author: Joseph Schornak
  */

#ifndef ARUCO_FINDER_H
#define ARUCO_FINDER_H

#include <boost/optional.hpp>
#include <opencv2/core.hpp>
#include <opencv2/aruco.hpp>
#include <rct_optimizations/types.h>
#include <map>
#include <Eigen/Core>
#include <iostream>

namespace rct_image_tools
{

/**
 * @brief This class finds 2D observations from images of a specified ArUco gridboard target.
 * The main advantage of this kind of target is that partial views still provide usable correspondences.
 * Observations are returned as a map where the marker ID is the key and the image coordinates of the
 * marker corners are the mapped value.
 */
class ArucoGridBoardObservationFinder
{
public:
  ArucoGridBoardObservationFinder(const cv::Ptr<cv::aruco::GridBoard>& board);

  /**
   * @brief findObservations Detect marker corner coordinates in the provided image.
   * @param image Input image, ideally containing an ArUco gridboard.
   * @return Map matching marker ID numbers to a vector of marker corner coordinates. The vector will contain
   * four corners defined in the same order as in the output of the function cv::aruco::DetectMarkers()
   * (e.g. clockwise from the "origin" corner).
   */
  boost::optional<std::map<int, std::vector<Eigen::Vector2d>>> findObservations(const cv::Mat& image) const;

  /**
   * @brief A debugging utility that will draw an observation set onto a copy of a cv::Mat for
   * display purposes. Usually you want to call findObservations() above then this with the result.
   */
  cv::Mat drawObservations(const cv::Mat& image, const std::map<int, std::vector<Eigen::Vector2d>>& observations) const;

  const cv::Ptr<cv::aruco::GridBoard>& target() const { return board_; }

private:
  cv::Ptr<cv::aruco::GridBoard> board_;
};

/**
 * @brief mapArucoIdsToObjPts Utility function to map marker ID values to the spatial coordinates of the corners
 * defined by the definition of the gridboard. This is used to generate a set of "object points" which can be used
 * in conjunction with the set of "image points" generated by findObservations to match 2D image coordinates with
 * corresponding 3D object coordinates.
 * @param board ArUco gridboard to use.
 * @return Map matching marker ID numbers to a vector of marker corner coordinates. The vector will contain
 * four corners defined in the same order as in the output of the function cv::aruco::DetectMarkers()
 * (e.g. clockwise from the "origin" corner).
 */
std::map<int, std::vector<Eigen::Vector3d>>
mapArucoIdsToObjPts(const cv::Ptr<cv::aruco::GridBoard>& board);

}
#endif // ARUCO_FINDER_H
