#ifndef KINEMATICS_H
#define KINEMATICS_H

#include <Arduino.h>


namespace IK {

	// Utility Functions
	inline float    rad2Deg(float rad) { return rad * 180.0f / M_PI; }              // Convert radians [M_PI, -M_PI] to degrees [180, -180]
	inline float    deg2Rad(float deg) { return deg * M_PI / 180.0f; }              // Convert degrees [-180, 180] to radians [-M_PI, M_PI]
	float           wrap360(float deg);                                             // Wrap angle from [180, -180] to [0, 360]
	bool            deg2Tick(float deg, uint16_t &tick);                            // Convert degrees [30, 330] to ticks [0, 1023]
	bool            tick2Deg(uint16_t tick, float &deg);                            // Convert ticks [0, 1023] to degrees [30, 330]

	// Inverse Kinematics
	bool getIKLocal(float tip_local_x, float tip_local_y, float tip_local_z, float baseR, uint16_t* positions);
	bool getIKGlobal(float tip_global_x, float tip_global_y, float tip_global_z, float baseX, float baseY, float baseZ, float baseR, uint16_t* positions);

	// Forward Kinematics
	bool getFKLocal(uint16_t coxa, uint16_t femur, uint16_t tibia, float baseR, float* tip_local_x, float* tip_local_y, float* tip_local_z);
	bool getFKGlobal(uint16_t coxa, uint16_t femur, uint16_t tibia, float baseX, float baseY, float baseZ, float baseR, float* tip_global_x, float* tip_global_y, float* tip_global_z);

	// Coordinate Transformations
	void global2Local(float global_x, float global_y, float global_z, float baseX, float baseY, float baseZ, float* local_x, float* local_y, float* local_z);
	void local2Global(float local_x, float local_y, float local_z, float baseX, float baseY, float baseZ, float* global_x, float* global_y, float* global_z);


}

#endif // KINEMATICS_H