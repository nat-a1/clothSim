#include "utils.h"


double closest_point_segment(vec3& sp1, vec3& sp2, vec3& tp1, vec3& tp2, double& sc, double& tc)
{
	double SMALL_NUM = 0.0001;
	vec3   u = sp2 - sp1;
	vec3   v = tp2 - tp1;
	vec3   w = sp1 - tp1;
	double    a = dot(u, u);         // always >= 0
	double    b = dot(u, v);
	double    c = dot(v, v);         // always >= 0
	double    d = dot(u, w);
	double    e = dot(v, w);
	double    D = a * c - b * b;        // always >= 0
	double    sN, sD = D;       // sc = sN / sD, default sD = D >= 0
	double    tN, tD = D;       // tc = tN / tD, default tD = D >= 0

	// compute the line parameters of the two closest points
	if (D < SMALL_NUM) { // the lines are almost parallel
		sN = 0.0;         // force using point P0 on segment S1
		sD = 1.0;         // to prevent possible division by 0.0 later
		tN = e;
		tD = c;
	}
	else {                 // get the closest points on the infinite lines
		sN = (b * e - c * d);
		tN = (a * e - b * d);
		if (sN < 0.0) {        // sc < 0 => the s=0 edge is visible
			sN = 0.0;
			tN = e;
			tD = c;
		}
		else if (sN > sD) {  // sc > 1  => the s=1 edge is visible
			sN = sD;
			tN = e + b;
			tD = c;
		}
	}

	if (tN < 0.0) {            // tc < 0 => the t=0 edge is visible
		tN = 0.0;
		// recompute sc for this edge
		if (-d < 0.0)
			sN = 0.0;
		else if (-d > a)
			sN = sD;
		else {
			sN = -d;
			sD = a;
		}
	}
	else if (tN > tD) {      // tc > 1  => the t=1 edge is visible
		tN = tD;
		// recompute sc for this edge
		if ((-d + b) < 0.0)
			sN = 0;
		else if ((-d + b) > a)
			sN = sD;
		else {
			sN = (-d + b);
			sD = a;
		}
	}
	// finally do the division to get sc and tc
	sc = (abs(sN) < SMALL_NUM ? 0.0 : sN / sD);
	tc = (abs(tN) < SMALL_NUM ? 0.0 : tN / tD);


	// get the difference of the two closest points
	vec3 dP = w + (u * (float)sc) - (v * (float)tc);  // =  S1(sc) - S2(tc)

	return  length(dP);   // return the closest distance
}
