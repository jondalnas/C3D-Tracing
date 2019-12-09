#pragma once

#include "Vec3.h"
#include "Material.h"
#include <memory>
#include <random>

class Math {
public:
    static Vec3 reflect(Vec3 dir, Vec3 normal) {
        return dir - normal * 2 * dir.dot(normal);
    }

    static Vec3 refract(Vec3 dir, Vec3 normal, double refractiveIndexOfRay, double refractiveIndexOfObject) {
        double cosDir = dir.dot(normal);

        double refractiveIndex = refractiveIndexOfRay / refractiveIndexOfObject;

        if (cosDir < 0) cosDir = -cosDir;
        else normal = normal * -1;

        double k = 1 - refractiveIndex * refractiveIndex * (1 - cosDir * cosDir);
        if (k < 0) return Vec3();
        else return dir * refractiveIndex + normal * (refractiveIndex * cosDir - sqrt(k));
    }

    static double fresnel(Vec3 dir, Vec3 normal, double refractiveIndexOfRay, double refractiveIndexOfObject) {
        double cosDir = dir.dot(normal);

        double sinTargetSqrt = (refractiveIndexOfRay * refractiveIndexOfRay) / (refractiveIndexOfObject * refractiveIndexOfObject) * (1 - cosDir * cosDir);

        if (sinTargetSqrt >= 1) {
            return 1;
        } else {
            double cosTarget = sqrt(1 - sinTargetSqrt);
            cosDir = fabs(cosDir);
            double rs = ((refractiveIndexOfObject * cosDir) - (refractiveIndexOfRay * cosTarget)) / ((refractiveIndexOfObject * cosDir) + (refractiveIndexOfRay * cosTarget));
            double rp = ((refractiveIndexOfRay * cosDir) - (refractiveIndexOfObject * cosTarget)) / ((refractiveIndexOfRay * cosDir) + (refractiveIndexOfObject * cosTarget));

            return (rs * rs + rp * rp) / 2;
        }
    }

    static Vec3 hemisphere(Vec3 normal, std::mt19937 &rng) {
        std::uniform_real_distribution<> unit(0.0, 1.0);
        //Manually calculate tangent and bitangent based on the cross product of the vector (1, 0, 0) and the normal
        //and then the cross product of that vector and the normal
        //n x (1, 0, 0) = (n.y * 0 - n.z * 0, n.z * 1 - n.x * 0, n.x * 0 - n.y * 1)
        Vec3 tangent(abs(normal.x) == 1 ? Vec3(0, 0, 1) : Vec3(0, normal.z, -normal.y));
        Vec3 bitangent(abs(normal.x) == 1 ? Vec3(0, -1, 0) : Vec3(-normal.y * normal.y - normal.z * normal.z, normal.x * normal.y, normal.x * normal.z).normalize());

        double r = sqrt(unit(rng));
        double theta = unit(rng) * 2.0 * M_PI;

        double x = r * cos(theta);
        double y = r * sin(theta);
        double z = sqrt(1.0 - (x * x + y * y));

        return (normal * z + tangent * x + bitangent * y).normalize();
    }

	static double BRDF(Vec3 view, Vec3 reflecRay, Vec3 normal, Material *mat, double refractiveIndexOfRay) {
		auto halfVector = view + reflecRay;
		halfVector.normalize();

		auto viewTheta = view.dot(normal);
		auto reflectTheta = reflecRay.dot(normal);
		auto halfTheta = halfVector.dot(normal);
		auto viewHalfTheta = view.dot(halfVector);

		return distributionBRDF(halfTheta, mat->roughness) * fresnelBRDF(viewHalfTheta, refractiveIndexOfRay, mat->refractiveIndex) * geometricBRDF(halfTheta, viewTheta, reflectTheta, viewHalfTheta) / (M_PI * viewTheta * reflectTheta);
	}

	static double geometricBRDF(double halfTheta, double viewTheta, double reflectTheta, double viewHalfTheta) {
		//G = min(1,(2*(H N)*(V N))/(V H),(2*(H N)*(L N))/(V H))
		//Let a be a=(2*(H N)/(V H)) then G = min(1,a*(V N),a*(L N))
		//Thus we only need to check if (V N) if greater than (L N)
		auto a = (2 * halfTheta) / viewHalfTheta;
		
		auto G = a;
		if (viewTheta > reflectTheta)
			G *= reflectTheta;
		else
			G *= viewTheta;

		return std::min(1.0, G);
	}

	//This one is wrong, because it results in k_spec and not D, which is the one we need to compute Cook-Torrance BRDF
	//This can be used on its own, so I'm still keeping it
	static double distributionBRDF(double halfTheta, double roughness) {
		//Using Beckmann distribution
		//k_spec=exp(-tan^2(alpha)/m^2)/(pi*m^2*cos^4(alpha)), alpha=arccos(N H)
		//tan^2(alpha)/m^2 = (1-cos^2(alpha))/(cos^2(alpha)*m^2)
		//k_spec=exp(-(1-cos^2(alpha))/(cos^2(alpha)*m^2))/(pi*m^2*cos^4(alpha)), alpha=arccos(N H)
		//cos^2(arccos(N H)) = (N H)^2, -1<=x<=1
		//k_spec=exp(-(1-(N H)^2)/((N H)^2*m^2))/(pi*m^2*(N H)^4)

		auto halfTheta2 = halfTheta * halfTheta;
		auto roughness2 = roughness * roughness;

		return std::exp(-(1-halfTheta2)/(halfTheta2 * roughness2)) / (M_PI * roughness2 * halfTheta2 * halfTheta2);
	}

	static double fresnelBRDF(double viewHalfTheta, double refractiveIndexOfRay, double refractiveIndexOfObject) {
		//Using Slicks's approximation
		//R(theta)=R_0+(1-R_0)*(1-cos(theta))^5
		//R_0=((n_1-n_2)/(n_1+n_2))
		//theta is (V H) because we are using microfacet moddeling

		auto r0 = (refractiveIndexOfRay - refractiveIndexOfObject) / (refractiveIndexOfRay + refractiveIndexOfObject);
		return r0 + (1 - r0) * std::pow(1 - viewHalfTheta, 5);
	}
};
