#pragma once
#include "Utils.h"

//cheks if two line segments intersect
bool lineSegmentIntersection(std::pair<double, double> p, std::pair<double, double> p1, std::pair<double, double> p2, std::pair<double, double> p3) {
	//-1 = counterclockwise, 0 = colinear, 1 = clockwise
	double o, o1, o2, o3;

	//get the orientations of the triads
	o = (p1.second - p.second) * (p2.first - p1.first) - (p1.first - p.first) * (p2.second - p1.second);
	o1 = (p1.second - p.second) * (p3.first - p1.first) - (p1.first - p.first) * (p3.second - p1.second);
	o2 = (p3.second - p2.second) * (p.first - p3.first) - (p3.first - p2.first) * (p.second - p3.second);
	o3 = (p3.second - p2.second) * (p1.first - p3.first) - (p3.first - p2.first) * (p1.second - p3.second);
	
	//map into the right space
	if(o) o = (o && o > 0) ? -1 : 1;
	if (o1) o1 = (o1 && o1 > 0) ? -1 : 1;
	if (o2) o2 = (o2 && o2 > 0) ? -1 : 1;
	if (o3) o3 = (o3 && o3 > 0) ? -1 : 1;

	//blatant intersection
	if (o != o1 && o2 != o3) return true;

	//colinearity
	if (!o && p2.first <= std::max(p.first, p1.first) && p2.first >= std::min(p.first, p1.first) && p2.second <= std::max(p.second, p1.second) && p2.second >= std::min(p.second, p1.second)) return true;
	if (!o1 && p3.first <= std::max(p.first, p1.first) && p3.first >= std::min(p.first, p1.first) && p3.second <= std::max(p.second, p1.second) && p3.second >= std::min(p.second, p1.second)) return true;
	if (!o2 && p.first <= std::max(p2.first, p3.first) && p.first >= std::min(p2.first, p3.first) && p.second <= std::max(p2.second, p3.second) && p.second >= std::min(p2.second, p3.second)) return true;
	if (!o3 && p.first <= std::max(p1.first, p3.first) && p.first >= std::min(p1.first, p3.first) && p.second <= std::max(p1.second, p3.second) && p.second >= std::min(p1.second, p3.second)) return true;

	return false;
}

//checks if a rectangle intersects with an elipse
bool rectIntersectsEllipse(std::pair<double, double> rectTopLeft, double width, double height, std::pair<double, double> ellipseCenter, double widthRadius, double heightRadius) {
	//first check for rectangular intersections
	if (rectTopLeft.second + height >= ellipseCenter.second - heightRadius && rectTopLeft.second <= ellipseCenter.second + heightRadius &&
		rectTopLeft.first + width >= ellipseCenter.first - widthRadius && rectTopLeft.first <= ellipseCenter.first + widthRadius) {
		
		//check if the rectangle crosses either of the axes
		if (rectTopLeft.first < ellipseCenter.first && rectTopLeft.first + width > ellipseCenter.first) {
			return true;
		}
		if (rectTopLeft.second < ellipseCenter.second && rectTopLeft.second + height > ellipseCenter.second) {
			return true;
		}

		//check point by point

		//first find foci of ellipse
		std::pair<double, double> f1, f2;

		if (widthRadius > heightRadius) {
			double focalDist = sqrt(widthRadius * widthRadius - heightRadius * heightRadius);

			f1 = std::make_pair(ellipseCenter.first, ellipseCenter.second + focalDist);
			f2 = std::make_pair(ellipseCenter.first, ellipseCenter.second - focalDist);
		}
		else if (widthRadius < heightRadius) {
			double focalDist = sqrt(-widthRadius * widthRadius + heightRadius * heightRadius);

			f1 = std::make_pair(ellipseCenter.first + focalDist, ellipseCenter.second);
			f2 = std::make_pair(ellipseCenter.first - focalDist, ellipseCenter.second);
		}
		else { // circle
			if (dist(&rectTopLeft, &ellipseCenter) < widthRadius) return true;
			if (dist(&std::make_pair(rectTopLeft.first, rectTopLeft.second + height), &ellipseCenter) < widthRadius) return true;
			if (dist(&std::make_pair(rectTopLeft.first + width, rectTopLeft.second + height), &ellipseCenter) < widthRadius) return true;
			if (dist(&std::make_pair(rectTopLeft.first + width, rectTopLeft.second), &ellipseCenter) < widthRadius) return true;

			return false;
		}

		double majorAxisLength = (widthRadius > heightRadius) ? 2 * widthRadius : 2 * heightRadius;

		//test distances from foci
		if (dist(&rectTopLeft, &f1) + dist(&rectTopLeft, &f2) < majorAxisLength) return true;
		if (dist(&std::make_pair(rectTopLeft.first, rectTopLeft.second + height), &f1) + dist(&std::make_pair(rectTopLeft.first, rectTopLeft.second + height), &f2) < majorAxisLength) return true;
		if (dist(&std::make_pair(rectTopLeft.first + width, rectTopLeft.second + height), &f1) + dist(&std::make_pair(rectTopLeft.first + width, rectTopLeft.second + height), &f2) < majorAxisLength) return true;
		if (dist(&std::make_pair(rectTopLeft.first + width, rectTopLeft.second), &f1) + dist(&std::make_pair(rectTopLeft.first + width, rectTopLeft.second), &f2) < majorAxisLength) return true;
	}
	return false;
}