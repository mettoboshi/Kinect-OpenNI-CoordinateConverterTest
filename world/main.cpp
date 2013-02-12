#include <iostream>

#include <opencv2\opencv.hpp>
#include <OpenNI.h>


void main()
{

	cv::namedWindow("Depth Camera", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
	cv::namedWindow("Point Camera", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);

    try {
		openni::OpenNI::initialize();
		
		openni::Device device;
        auto ret = device.open( openni::ANY_DEVICE );
        if ( ret != openni::STATUS_OK ) {
            throw std::runtime_error( "" );
        }

		openni::VideoStream depthStream;
        depthStream.create( device, openni::SensorType::SENSOR_DEPTH );
        depthStream.start();

		device.setDepthColorSyncEnabled(true);
		device.setImageRegistrationMode(openni::IMAGE_REGISTRATION_DEPTH_TO_COLOR);

		cv::Mat depthImage;
		cv::Mat dst_image;

		//worldç¿ïW
		cv::Point3f pointw[100][100];
		for (int i = 0; i < 100; i++) {
			for (int j = 0; j < 100; j++) {
				pointw[i][j].x = 100 + i;
				pointw[i][j].y = 100 + j;
				pointw[i][j].z = 500;
				}
		}

		//worldç¿ïW2
		cv::Point3f pointRect[8];
		pointRect[0].x = -200;
		pointRect[0].y = -200;
		pointRect[0].z = 500;

		pointRect[1].x = -100;
		pointRect[1].y = -200;
		pointRect[1].z = 500;

		pointRect[2].x = -200;
		pointRect[2].y = -100;
		pointRect[2].z = 500;

		pointRect[3].x = -100;
		pointRect[3].y = -100;
		pointRect[3].z = 500;

   		pointRect[4].x = -200;
		pointRect[4].y = -200;
		pointRect[4].z = 600;

		pointRect[5].x = -100;
		pointRect[5].y = -200;
		pointRect[5].z = 600;

		pointRect[6].x = -200;
		pointRect[6].y = -100;
		pointRect[6].z = 600;

		pointRect[7].x = -100;
		pointRect[7].y = -100;
		pointRect[7].z = 600;





		cv::Point3f pointp[100][100];
		cv::Point3f pointpr[8];

		int depthNum = 10;

		while ( 1 ) {

			// OpenNIÇÃdepthèÓïÒÇì«Ç›çûÇﬁ
			openni::VideoFrameRef depthFrame;
			depthStream.readFrame( &depthFrame );

			if ( depthFrame.isValid() ) {
					// DepthèÓïÒÇéÊìæ
                    depthImage = cv::Mat( depthStream.getVideoMode().getResolutionY(),
                                          depthStream.getVideoMode().getResolutionX(),
                                          CV_16U, (char*)depthFrame.getData() );
			}

/*
openni::CoordinateConverter::convertDepthToWorld	(	const VideoStream & 	depthStream,
float 	depthX,
float 	depthY,
float 	depthZ,
float * 	pWorldX,
float * 	pWorldY,
float * 	pWorldZ 
)
*/
			cv::Mat pointImage = cv::Mat::zeros(cv::Size(640, 480), CV_8UC3);

			if(pointw[0][0].z > 1000 && depthNum > 0) {
				depthNum = -10;
			} else if(pointw[0][0].z < 500 && depthNum < 0) {
				depthNum = 10;
			}

			for (int i = 0; i < 100; i++) {
				for(int j = 0; j < 100; j++) {
					pointw[i][j].z += depthNum;
					openni::CoordinateConverter::convertWorldToDepth(depthStream,
																	 pointw[i][j].x,
																	 pointw[i][j].y,
																	 pointw[i][j].z,
																	 &pointp[i][j].x,
																	 &pointp[i][j].y,
																	 &pointp[i][j].z);
				}
			}

			for (int i = 0; i < 8; i++) {
				pointRect[i].z += depthNum;
				openni::CoordinateConverter::convertWorldToDepth(depthStream,
																 pointRect[i].x,
																 pointRect[i].y,
																 pointRect[i].z,
																 &pointpr[i].x,
																 &pointpr[i].y,
																 &pointpr[i].z);
			}
			
			


/*
			cv::Point3f pointp[100][100];
			for (int i = 0; i < 100; i++) {
				for (int j = 0; j < 100; j++) {
					pointp[i][j].x = pointw[i][j].x;
					pointp[i][j].y = pointw[i][j].y;
					pointp[i][j].z = pointw[i][j].z;
				}
			}
*/
			for (int i = 0; i < 100; i++) {
				for (int j = 0; j < 100; j++) {
					cv::circle(pointImage, cv::Point(pointp[i][j].x, pointp[i][j].y), 1, cv::Scalar(0,0,255), -1, CV_AA);
				}
			}


			cv::line(pointImage, cv::Point2f(pointpr[0].x,pointpr[0].y), 
				                 cv::Point2f(pointpr[4].x,pointpr[4].y),
							     cv::Scalar(100, 100, 200), 2, CV_AA);

			cv::line(pointImage, cv::Point2f(pointpr[1].x,pointpr[1].y), 
				                 cv::Point2f(pointpr[5].x,pointpr[5].y),
							     cv::Scalar(100, 100, 200), 2, CV_AA);

			cv::line(pointImage, cv::Point2f(pointpr[2].x,pointpr[2].y), 
				                 cv::Point2f(pointpr[6].x,pointpr[6].y),
							     cv::Scalar(100, 100, 200), 2, CV_AA);

			cv::line(pointImage, cv::Point2f(pointpr[3].x,pointpr[3].y), 
				                 cv::Point2f(pointpr[7].x,pointpr[7].y),
							     cv::Scalar(100, 100, 200), 2, CV_AA);

			cv::line(pointImage, cv::Point2f(pointpr[4].x,pointpr[4].y), 
				                 cv::Point2f(pointpr[5].x,pointpr[5].y),
							     cv::Scalar(100, 100, 200), 2, CV_AA);

			cv::line(pointImage, cv::Point2f(pointpr[4].x,pointpr[4].y), 
				                 cv::Point2f(pointpr[6].x,pointpr[6].y),
							     cv::Scalar(100, 100, 200), 2, CV_AA);

			cv::line(pointImage, cv::Point2f(pointpr[7].x,pointpr[7].y), 
				                 cv::Point2f(pointpr[5].x,pointpr[5].y),
							     cv::Scalar(100, 100, 200), 2, CV_AA);

			cv::line(pointImage, cv::Point2f(pointpr[7].x,pointpr[7].y), 
				                 cv::Point2f(pointpr[6].x,pointpr[6].y),
							     cv::Scalar(100, 100, 200), 2, CV_AA);

			cv::line(pointImage, cv::Point2f(pointpr[0].x,pointpr[0].y), 
				                 cv::Point2f(pointpr[1].x,pointpr[1].y),
							     cv::Scalar(255, 0, 0), 2, CV_AA);

			cv::line(pointImage, cv::Point2f(pointpr[0].x,pointpr[0].y), 
				                 cv::Point2f(pointpr[2].x,pointpr[2].y),
							     cv::Scalar(255, 0, 0), 2, CV_AA);

			cv::line(pointImage, cv::Point2f(pointpr[1].x,pointpr[1].y), 
				                 cv::Point2f(pointpr[3].x,pointpr[3].y),
							     cv::Scalar(255, 0, 0), 2, CV_AA);

			cv::line(pointImage, cv::Point2f(pointpr[2].x,pointpr[2].y), 
				                 cv::Point2f(pointpr[3].x,pointpr[3].y),
							     cv::Scalar(255, 0, 0), 2, CV_AA);


			std::stringstream string1;
            string1 << "depth : " << pointp[0][0].z;
			cv::putText( pointImage,
				         string1.str(),
				         cv::Point( 0, 50 ),
                         cv::FONT_HERSHEY_SIMPLEX,
						 1.0,
						 cv::Scalar( 255, 0, 0 ),
						 1 );
			


//			float wx[100] = {0};
//			float wy[100] = {0};
//			float wz[100] = {0};

			//depthç¿ïW
			float dx[100] = {0};
			float dy[100] = {0};
			float dz[100] = {0};



			float x = 0;
			float y = 0; 
			float z = 0;

			float xx = 0;
			float yy = 0; 
			float zz = 0;

			float xxx = 0;
			float yyy = 0;
			float zzz = 0;



			openni::CoordinateConverter::convertDepthToWorld(depthStream,
															 400,
															 400,
															 depthImage.at<openni::DepthPixel>(400,400),
															 &x,
															 &y,
															 &z);



			//cv::circle(pointImage, cv::Point(200, 200), 5, cv::Scalar(0,0,200), -1, CV_AA);

			openni::CoordinateConverter::convertDepthToWorld(depthStream,
															 200,
															 200,
															 depthImage.at<openni::DepthPixel>(200,200),
															 &xx,
															 &yy,
															 &zz);


			//cv::circle(pointImage, cv::Point(320, 240), 5, cv::Scalar(0,0,200), -1, CV_AA);

			openni::CoordinateConverter::convertDepthToWorld(depthStream,
															 320,
															 240,
															 depthImage.at<openni::DepthPixel>(320,240),
															 &xxx,
															 &yyy,
															 &zzz);


//			std::cout << "x0 : " << x[0] << "y0 : " << y[0] << "z0" << z[0] << std::endl;
//			std::cout << "x1 : " << x[1] << "y1 : " << y[1] << "z1" << z[1] << std::endl;
//			std::cout << "x2 : " << x[2] << "y2 : " << y[2] << "z2" << z[2] << std::endl;


			depthImage.convertTo( depthImage, CV_8U, 255.0 / 10000 );
			cv::cvtColor(depthImage, dst_image, CV_GRAY2BGR);

			cv::circle(dst_image, cv::Point(400, 400), 5, cv::Scalar(200,0,0), -1, CV_AA);
			cv::circle(dst_image, cv::Point(200, 200), 5, cv::Scalar(200,0,0), -1, CV_AA);
			cv::circle(dst_image, cv::Point(320, 240), 5, cv::Scalar(200,0,0), -1, CV_AA);


			auto videoMode = depthStream.getVideoMode();

			int centerX = 200;
            int centerY = 200;
            int centerIndex = (centerY * videoMode.getResolutionX()) + centerX;

			short* depth = (short*)depthFrame.getData();


			std::stringstream ss;
            ss << "400, 400 :" << depth[centerIndex];
			cv::putText( dst_image,
				         ss.str(),
				         cv::Point( 0, 50 ),
                         cv::FONT_HERSHEY_SIMPLEX,
						 1.0,
						 cv::Scalar( 255, 0, 0 ),
						 1 );

			std::stringstream ss2;
			ss2 << "x :" << x << " y :"<< y << " z :"<< z;

			cv::putText( dst_image, 
				         ss2.str(), 
						 cv::Point( 0, 100 ),
                         cv::FONT_HERSHEY_SIMPLEX, 
						 1.0, 
						 cv::Scalar( 255, 0, 0 ), 
						 1 );




			std::stringstream ss3;
            ss3 << "200, 200 :" << depth[centerIndex];
			cv::putText( dst_image,
				         ss3.str(),
				         cv::Point( 0, 150 ),
                         cv::FONT_HERSHEY_SIMPLEX,
						 1.0,
						 cv::Scalar( 0, 255, 0 ),
						 1 );

			std::stringstream ss4;
			ss4 << "x :" << xx << " y :"<< yy << " z :"<< zz;

			cv::putText( dst_image, 
				         ss4.str(), 
						 cv::Point( 0, 200 ),
                         cv::FONT_HERSHEY_SIMPLEX, 
						 1.0, 
						 cv::Scalar( 0, 255, 0 ), 
						 1 );


			std::stringstream ss5;
            ss5 << "320, 240 :" << depth[centerIndex];
			cv::putText( dst_image,
				         ss5.str(),
				         cv::Point( 0, 250 ),
                         cv::FONT_HERSHEY_SIMPLEX,
						 1.0,
						 cv::Scalar( 0, 0, 255 ),
						 1 );

			std::stringstream ss6;
			ss6 << "x :" << xxx << " y :"<< yyy << " z :"<< zzz;

			cv::putText( dst_image, 
				         ss6.str(), 
						 cv::Point( 0, 300 ),
                         cv::FONT_HERSHEY_SIMPLEX, 
						 1.0, 
						 cv::Scalar( 0, 0, 255 ), 
						 1 );



//			cv::add(pointImage, dst_image, dst_image);

			cv::imshow( "Depth Camera", dst_image );
			cv::imshow( "Point Camera", pointImage );


            int key = cv::waitKey( 10 );
            if ( key == 'q' ) {
                break;
            }
        }
    }
    catch ( std::exception& ) {
        std::cout << openni::OpenNI::getExtendedError() << std::endl;
    }
}