#pragma once

bool lineSegmentIntersection(std::pair<double, double> p, std::pair<double, double> p1, std::pair<double, double> p2, std::pair<double, double> p3) {
	//-1 = counterclockwise, 0 = colinear, 1 = clockwise
	char o, o1, o2, o3;

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