#pragma once

#include <boost/container/vector.hpp>
#include "ModelComponent.h"

class Chunk : public ModelComponent {
public:
	const boost::container::vector<bool> data;
	Chunk(const unsigned char &width, const unsigned char &height, const unsigned char &depth, const Point3 &blockSize, const boost::container::vector<bool> &data) : data(data) {
		type = GeometryType::Triangles;
		const std::vector<Triangle> blockLeft = {
			std::make_tuple(Point3(-0.5, -0.5, -0.5), Point3(-0.5, -0.5,  0.5), Point3(-0.5,  0.5, -0.5)),
			std::make_tuple(Point3(-0.5, -0.5,  0.5), Point3(-0.5,  0.5,  0.5), Point3(-0.5,  0.5, -0.5))
		};
		const std::vector<Triangle> blockRight = {
			std::make_tuple(Point3( 0.5, -0.5,  0.5), Point3( 0.5, -0.5, -0.5), Point3( 0.5,  0.5,  0.5)),
			std::make_tuple(Point3( 0.5, -0.5, -0.5), Point3( 0.5,  0.5, -0.5), Point3( 0.5,  0.5,  0.5))
		};
		const std::vector<Triangle> blockBottom = {
			std::make_tuple(Point3(-0.5, -0.5, -0.5), Point3( 0.5, -0.5, -0.5), Point3(-0.5, -0.5,  0.5)),
			std::make_tuple(Point3( 0.5, -0.5, -0.5), Point3( 0.5, -0.5,  0.5), Point3(-0.5, -0.5,  0.5))
		};
		const std::vector<Triangle> blockTop = {
			std::make_tuple(Point3(-0.5,  0.5,  0.5), Point3( 0.5,  0.5,  0.5), Point3(-0.5,  0.5, -0.5)),
			std::make_tuple(Point3( 0.5,  0.5,  0.5), Point3( 0.5,  0.5, -0.5), Point3(-0.5,  0.5, -0.5))
		};
		const std::vector<Triangle> blockBack = {
			std::make_tuple(Point3(-0.5,  0.5, -0.5), Point3( 0.5,  0.5, -0.5), Point3(-0.5, -0.5, -0.5)),
			std::make_tuple(Point3( 0.5,  0.5, -0.5), Point3( 0.5, -0.5, -0.5), Point3(-0.5, -0.5, -0.5))
		};
		const std::vector<Triangle> blockFront = {
			std::make_tuple(Point3(-0.5, -0.5,  0.5), Point3( 0.5, -0.5,  0.5), Point3(-0.5,  0.5,  0.5)),
			std::make_tuple(Point3( 0.5, -0.5,  0.5), Point3( 0.5,  0.5,  0.5), Point3(-0.5,  0.5,  0.5))
		};

		points.reserve(width * height * depth * 3);

		for(unsigned char x = 0; x < width; ++x) {
			for(unsigned char y = 0; y < height; ++y) {
				for(unsigned char z = 0; z < depth; ++z) {
					auto current = z * width * height + x * height + y;
					if(data.at(current)) {
						Point3 offset(x + 0.5f, y + 0.5f, z + 0.5f);

						bool left   = x > 0          && data.at(current - height);
						bool right  = x < width - 1  && data.at(current + height);
						bool bottom = y > 0          && data.at(current - 1);
						bool top    = y < height - 1 && data.at(current + 1);
						bool back   = z > 0          && data.at(current - width * height);
						bool front  = z < depth - 1  && data.at(current + width * height);

						/*if(!left && right) {
							if(!bottom && top) {
								if(!back && front) {
									points.emplace_back(Point3(-0.5,  0.5,  0.5) + offset);
									points.emplace_back(Point3( 0.5,  0.5, -0.5) + offset);
									points.emplace_back(Point3( 0.5, -0.5,  0.5) + offset);
								} else if(!front && back) {
									points.emplace_back(Point3(-0.5,  0.5, -0.5) + offset);
									points.emplace_back(Point3( 0.5, -0.5, -0.5) + offset);
									points.emplace_back(Point3( 0.5,  0.5,  0.5) + offset);
								} else {
									points.emplace_back(Point3(-0.5,  0.5,  0.5) + offset);
									points.emplace_back(Point3(-0.5,  0.5, -0.5) + offset);
									points.emplace_back(Point3( 0.5, -0.5, -0.5) + offset);
									points.emplace_back(Point3( 0.5, -0.5, -0.5) + offset);
									points.emplace_back(Point3( 0.5, -0.5,  0.5) + offset);
									points.emplace_back(Point3(-0.5,  0.5,  0.5) + offset);
								}
								continue;
							} else if(!top && bottom) {
								if(!back && front) {
									points.emplace_back(Point3(-0.5, -0.5,  0.5) + offset);
									points.emplace_back(Point3( 0.5,  0.5,  0.5) + offset);
									points.emplace_back(Point3( 0.5, -0.5, -0.5) + offset);
								} else if(!front && back) {
									points.emplace_back(Point3(-0.5, -0.5, -0.5) + offset);
									points.emplace_back(Point3( 0.5, -0.5,  0.5) + offset);
									points.emplace_back(Point3( 0.5,  0.5, -0.5) + offset);
								} else {
									points.emplace_back(Point3(-0.5, -0.5, -0.5) + offset);
									points.emplace_back(Point3(-0.5, -0.5,  0.5) + offset);
									points.emplace_back(Point3( 0.5,  0.5,  0.5) + offset);
									points.emplace_back(Point3( 0.5,  0.5,  0.5) + offset);
									points.emplace_back(Point3( 0.5,  0.5, -0.5) + offset);
									points.emplace_back(Point3(-0.5, -0.5, -0.5) + offset);
								}
								continue;
							} else if(!back && front) {
								points.emplace_back(Point3(-0.5, -0.5,  0.5) + offset);
								points.emplace_back(Point3(-0.5,  0.5,  0.5) + offset);
								points.emplace_back(Point3( 0.5,  0.5, -0.5) + offset);
								points.emplace_back(Point3( 0.5,  0.5, -0.5) + offset);
								points.emplace_back(Point3( 0.5, -0.5, -0.5) + offset);
								points.emplace_back(Point3(-0.5, -0.5,  0.5) + offset);
								continue;
							} else if(!front && back) {
								points.emplace_back(Point3(-0.5,  0.5, -0.5) + offset);
								points.emplace_back(Point3(-0.5, -0.5, -0.5) + offset);
								points.emplace_back(Point3( 0.5, -0.5,  0.5) + offset);
								points.emplace_back(Point3( 0.5, -0.5,  0.5) + offset);
								points.emplace_back(Point3( 0.5,  0.5,  0.5) + offset);
								points.emplace_back(Point3(-0.5,  0.5, -0.5) + offset);
								continue;
							}
						} else if(!right && left) {
							if(!bottom && top) {
								if(!back && front) {
									points.emplace_back(Point3( 0.5,  0.5,  0.5) + offset);
									points.emplace_back(Point3(-0.5, -0.5,  0.5) + offset);
									points.emplace_back(Point3(-0.5,  0.5, -0.5) + offset);
								} else if(!front && back) {
									points.emplace_back(Point3( 0.5,  0.5, -0.5) + offset);
									points.emplace_back(Point3(-0.5,  0.5,  0.5) + offset);
									points.emplace_back(Point3(-0.5, -0.5, -0.5) + offset);
								} else {
									points.emplace_back(Point3( 0.5,  0.5, -0.5) + offset);
									points.emplace_back(Point3( 0.5,  0.5,  0.5) + offset);
									points.emplace_back(Point3(-0.5, -0.5,  0.5) + offset);
									points.emplace_back(Point3(-0.5, -0.5,  0.5) + offset);
									points.emplace_back(Point3(-0.5, -0.5, -0.5) + offset);
									points.emplace_back(Point3( 0.5,  0.5, -0.5) + offset);
								}
								continue;
							} else if(!top && bottom) {
								if(!back && front) {
									points.emplace_back(Point3( 0.5,  0.5,  0.5) + offset);
									points.emplace_back(Point3( 0.5, -0.5, -0.5) + offset);
									points.emplace_back(Point3(-0.5,  0.5, -0.5) + offset);
								} else if(!front && back) {
									points.emplace_back(Point3( 0.5,  0.5, -0.5) + offset);
									points.emplace_back(Point3(-0.5,  0.5,  0.5) + offset);
									points.emplace_back(Point3( 0.5, -0.5,  0.5) + offset);
								} else {
									points.emplace_back(Point3( 0.5, -0.5,  0.5) + offset);
									points.emplace_back(Point3( 0.5, -0.5, -0.5) + offset);
									points.emplace_back(Point3(-0.5,  0.5, -0.5) + offset);
									points.emplace_back(Point3(-0.5,  0.5, -0.5) + offset);
									points.emplace_back(Point3(-0.5,  0.5,  0.5) + offset);
									points.emplace_back(Point3( 0.5, -0.5,  0.5) + offset);
								}
								continue;
							} else if(!back && front) {
								points.emplace_back(Point3( 0.5,  0.5,  0.5) + offset);
								points.emplace_back(Point3( 0.5, -0.5,  0.5) + offset);
								points.emplace_back(Point3(-0.5, -0.5, -0.5) + offset);
								points.emplace_back(Point3(-0.5, -0.5, -0.5) + offset);
								points.emplace_back(Point3(-0.5,  0.5, -0.5) + offset);
								points.emplace_back(Point3( 0.5,  0.5,  0.5) + offset);
								continue;
							} else if(!front && back) {
								points.emplace_back(Point3( 0.5, -0.5, -0.5) + offset);
								points.emplace_back(Point3( 0.5,  0.5, -0.5) + offset);
								points.emplace_back(Point3(-0.5,  0.5,  0.5) + offset);
								points.emplace_back(Point3(-0.5,  0.5,  0.5) + offset);
								points.emplace_back(Point3(-0.5, -0.5,  0.5) + offset);
								points.emplace_back(Point3( 0.5, -0.5, -0.5) + offset);
								continue;
							}
						} else if(!bottom && top) {
							if(!back && front) {
								points.emplace_back(Point3( 0.5, -0.5,  0.5) + offset);
								points.emplace_back(Point3(-0.5, -0.5,  0.5) + offset);
								points.emplace_back(Point3(-0.5,  0.5, -0.5) + offset);
								points.emplace_back(Point3(-0.5,  0.5, -0.5) + offset);
								points.emplace_back(Point3( 0.5,  0.5, -0.5) + offset);
								points.emplace_back(Point3( 0.5, -0.5,  0.5) + offset);
								continue;
							} else if(!front && back) {
								points.emplace_back(Point3(-0.5, -0.5, -0.5) + offset);
								points.emplace_back(Point3( 0.5, -0.5, -0.5) + offset);
								points.emplace_back(Point3( 0.5,  0.5,  0.5) + offset);
								points.emplace_back(Point3( 0.5,  0.5,  0.5) + offset);
								points.emplace_back(Point3(-0.5,  0.5,  0.5) + offset);
								points.emplace_back(Point3(-0.5, -0.5, -0.5) + offset);
								continue;
							}
						} else if(!top && bottom) {
							if(!back && front) {
								points.emplace_back(Point3(-0.5,  0.5,  0.5) + offset);
								points.emplace_back(Point3( 0.5,  0.5,  0.5) + offset);
								points.emplace_back(Point3( 0.5, -0.5, -0.5) + offset);
								points.emplace_back(Point3( 0.5, -0.5, -0.5) + offset);
								points.emplace_back(Point3(-0.5, -0.5, -0.5) + offset);
								points.emplace_back(Point3(-0.5,  0.5,  0.5) + offset);
								continue;
							} else if(!front && back) {
								points.emplace_back(Point3( 0.5,  0.5, -0.5) + offset);
								points.emplace_back(Point3(-0.5,  0.5, -0.5) + offset);
								points.emplace_back(Point3(-0.5, -0.5,  0.5) + offset);
								points.emplace_back(Point3(-0.5, -0.5,  0.5) + offset);
								points.emplace_back(Point3( 0.5, -0.5,  0.5) + offset);
								points.emplace_back(Point3( 0.5,  0.5, -0.5) + offset);
								continue;
							}
						}*/

						if(!left) {
							for(auto &triangle : blockLeft) {
								points.emplace_back((std::get<0>(triangle) +offset) * blockSize.x);
								points.emplace_back((std::get<1>(triangle) +offset) * blockSize.y);
								points.emplace_back((std::get<2>(triangle) +offset) * blockSize.z);
							}
						}
						if(!right) {
							for(auto &triangle : blockRight) {
								points.emplace_back((std::get<0>(triangle) +offset) * blockSize.x);
								points.emplace_back((std::get<1>(triangle) +offset) * blockSize.y);
								points.emplace_back((std::get<2>(triangle) +offset) * blockSize.z);
							}
						}
						if(!bottom) {
							for(auto &triangle : blockBottom) {
								points.emplace_back((std::get<0>(triangle) +offset) * blockSize.x);
								points.emplace_back((std::get<1>(triangle) +offset) * blockSize.y);
								points.emplace_back((std::get<2>(triangle) +offset) * blockSize.z);
							}
						}
						if(!top) {
							for(auto &triangle : blockTop) {
								points.emplace_back((std::get<0>(triangle) +offset) * blockSize.x);
								points.emplace_back((std::get<1>(triangle) +offset) * blockSize.y);
								points.emplace_back((std::get<2>(triangle) +offset) * blockSize.z);
							}
						}
						if(!back) {
							for(auto &triangle : blockBack) {
								points.emplace_back((std::get<0>(triangle) +offset) * blockSize.x);
								points.emplace_back((std::get<1>(triangle) +offset) * blockSize.y);
								points.emplace_back((std::get<2>(triangle) +offset) * blockSize.z);
							}
						}
						if(!front) {
							for(auto &triangle : blockFront) {
								points.emplace_back((std::get<0>(triangle) +offset) * blockSize.x);
								points.emplace_back((std::get<1>(triangle) +offset) * blockSize.y);
								points.emplace_back((std::get<2>(triangle) +offset) * blockSize.z);
							}
						}
					}
				}
			}
		}
	}
};
