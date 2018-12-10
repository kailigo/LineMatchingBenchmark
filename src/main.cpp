#include <stdio.h>
#include <highgui.h>
#include <cv.h>
#include <math.h>
#include <iostream>
// #include "IO.h"
#include "OpenCVMatrixIO.h"

using namespace std;  
using namespace cv;

void concatenateTwoImgs(Mat mImg1, Mat mImg2, Mat &outImg)
{
	IplImage img1= IplImage(mImg1); 
	IplImage img2= IplImage(mImg2);
	IplImage* stacked = cvCreateImage( cvSize( img1.width + img2.width, max(img1.height, img2.height)), IPL_DEPTH_8U, img1.nChannels); 
	cvSetImageROI( stacked, cvRect( 0, 0, img1.width, img1.height ) ); 
	cvCopy(&img1, stacked);//, stacked, NULL ); 
	cvResetImageROI(stacked); 
	cvSetImageROI( stacked, cvRect(img1.width, 0, img2.width, img2.height) ); 
	cvCopy(&img2, stacked);//, stacked, NULL ); 
	cvResetImageROI(stacked); 
	outImg = Mat(stacked,0);
}

void visualizeResultReport(Mat img1, Mat img2, Mat lines1, Mat lines2, Mat obtainedMatches, vector<bool> vaccess, vector<vector<int>> vec1, vector<vector<int>> vec2, string imgName)
{
	Mat newImg;
	concatenateTwoImgs(img1, img2, newImg);	
	int cols = img1.cols;
	int num = 0;

	vector<vector<int>> vmatch;
	for (int i = 0; i < vec1.size(); i++)
	{
		vector<int> vser1 = vec1[i];
		vector<int> vser2 = vec2[i];	
		int nmatch = std::min(vser1.size(), vser2.size());		
		for (int j = 0; j < nmatch; j++)
		{
			vector<int> vint;
			vint.push_back(vser1[j]);
			vint.push_back(vser2[j]);
			vmatch.push_back(vint);		
		}				
	}		

	int R, G, B;
	map<int, int> vmap1, vmap2;
	pair<int, int> iipair;
	for (int i = 0; i < obtainedMatches.rows; i++)
	{		
		if (vaccess[i])
		{
			G = 255; R = 255; B = 0;		
		}
		else
		{
			G = 0; R = 255; B = 0;		
		}
		int ser1 = obtainedMatches.at<int>(i, 0);
		int ser2 = obtainedMatches.at<int>(i, 1);

		iipair.first = ser1;
		iipair.second = ser2;		
		vmap1.insert(iipair);

		iipair.first = ser2;
		iipair.second = ser1;		
		vmap2.insert(iipair);

		Point2f spt1(lines1.at<float>(ser1, 0), lines1.at<float>(ser1, 1)); 
		Point2f ept1(lines1.at<float>(ser1, 2), lines1.at<float>(ser1, 3)); 
		line(newImg, spt1, ept1, cvScalar(B,G,R), 1.0, CV_AA);
		Point2f spt2(lines2.at<float>(ser2, 0)+cols, lines2.at<float>(ser2, 1)); 
		Point2f ept2(lines2.at<float>(ser2, 2)+cols, lines2.at<float>(ser2, 3)); 
		line(newImg, spt2, ept2, cvScalar(B,G,R), 1.0, CV_AA);
		stringstream ss;		
		string str;
		ss<<i;
		ss>>str;
		Point2i midpt1, midpt2, tpt;
		tpt = spt1 + ept1;
		midpt1.x = (int) tpt.x/2;
		midpt1.y = (int) tpt.y/2;	
		tpt = spt2 + ept2;
		midpt2.x = (int) tpt.x/2;
		midpt2.y = (int) tpt.y/2;	
		putText(newImg, str, midpt1, FONT_HERSHEY_SIMPLEX, 0.3, cvScalar(B, G, R), 1, CV_AA);		
		putText(newImg, str, midpt2, FONT_HERSHEY_SIMPLEX, 0.3, cvScalar(B, G, R), 1, CV_AA);		

		num++;
	}		

	for (int i = 0; i < vmatch.size(); i++)
	{
		R = 0; G = 0; B = 255;
		int ser1 = vmatch[i].at(0);
		int ser2 = vmatch[i].at(1);
		if (vmap1.end() == vmap1.find(ser1) && vmap2.end() == vmap2.find(ser2) )
		{
			Point2f spt1(lines1.at<float>(ser1, 0), lines1.at<float>(ser1, 1)); 
			Point2f ept1(lines1.at<float>(ser1, 2), lines1.at<float>(ser1, 3)); 
			line(newImg, spt1, ept1, cvScalar(B,G,R), 1.0, CV_AA);
			Point2f spt2(lines2.at<float>(ser2, 0)+cols, lines2.at<float>(ser2, 1)); 
			Point2f ept2(lines2.at<float>(ser2, 2)+cols, lines2.at<float>(ser2, 3)); 
			line(newImg, spt2, ept2, cvScalar(B,G,R), 1.0, CV_AA);

			stringstream ss;		
			string str;
			ss<<num;
			ss>>str;
			num++;
			Point2i midpt1, midpt2, tpt;
			tpt = spt1 + ept1;
			midpt1.x = (int) tpt.x/2;
			midpt1.y = (int) tpt.y/2;	
			tpt = spt2 + ept2;
			midpt2.x = (int) tpt.x/2;
			midpt2.y = (int) tpt.y/2;	
			putText(newImg, str, midpt1, FONT_HERSHEY_SIMPLEX, 0.3, cvScalar(B, G, R), 1, CV_AA);		
			putText(newImg, str, midpt2, FONT_HERSHEY_SIMPLEX, 0.3, cvScalar(B, G, R), 1, CV_AA);		
		}		
	}
	
	imshow("Visualized Report (correct matches in yellow, incorrect matches in red, matches not found in blue)", newImg);
	waitKey();

	imwrite(imgName, newImg);
}


std::vector<std::string> split(std::string str, std::string pattern)  
{  
	std::string::size_type pos;  
	std::vector<std::string> result;  
	str+=pattern;
	int size=str.size();  

	for(int i=0; i<size; i++)  
	{  
		pos=str.find(pattern,i);  
		if(pos<size)  
		{  
			std::string s=str.substr(i,pos-i);  
			result.push_back(s);  
			i=pos+pattern.size()-1;  
		}  
	}  
	return result;  
}  

void getsers(string str, vector<int> &vser)
{
	str = str.substr(1, str.size()-1);	
	vector<string> lms = split(str, ","); 
	for (int i = 0; i < lms.size(); i++)
		vser.push_back(atoi(lms[i].c_str()));	
}

int readGroundTruth(string fileName, vector<vector<int>> &vec1, vector<vector<int>> &vec2)
{
	ifstream infile(fileName.c_str());
	string temp, pattern = " ";	
	int ntotal = 0;
	while(getline(infile, temp))
	{		
		vector<string> lms = split(temp, pattern);  		
		string str1 = lms[0];
		string str2 = lms[1];
		vector<int> tvec1, tvec2;
		getsers(str1, tvec1);
		getsers(str2, tvec2);
		vec1.push_back(tvec1);			
		vec2.push_back(tvec2);		
		ntotal += std::min(tvec1.size(), tvec2.size());
	}			
	return ntotal;
}

int resultEvaluation(Mat obtainedMatches, vector<vector<int>> gvec1, vector<vector<int>> gvec2, vector<bool> &vaccess)
{
	int nobtain = obtainedMatches.rows;
	//vector<bool> vaccess(nobtain,0);
	int ncorrect = 0;
	for (int i = 0; i < nobtain; i++)
	{
		int ser1 = obtainedMatches.at<int>(i, 0);
		int ser2 = obtainedMatches.at<int>(i, 1);
		for (int j = 0; j < gvec1.size(); j++)
		{
			vector<int> tvec1 = gvec1[j];
			for (int m = 0; m < tvec1.size(); m++)
			{
				if (ser1 == tvec1[m])
				{
					vector<int> tvec2 = gvec2[j];
					for (int n = 0; n < tvec2.size(); n++)
					{
						if (ser2 == tvec2[n])
						{
							//vcorrect.push_back(i);
							ncorrect += 1;
							vaccess[i] = 1;
							break;
						}
					}
					break;
				}
			}
		}
	}	
	return ncorrect;
}


int main(int argc, char** argv)
{		
// 	argv[1] = "benchmark\\bikes\\";
// 	argv[2] = "img1.ppm";
// 	argv[3] = "img6.ppm";
// 	argv[4] = "lsd1.txt";
// 	argv[5] = "lsd6.txt";
// 	argv[6] = "lsd";
// 	argv[7] = "benchmark\\bikes\\ljlMatches1.txt";
	if (argc !=8)
	{
		cout<<"The parameters are: "<<endl;
		cout<<"(P1): the software name."<<endl;
		cout<<"(P2): the folder storing data for a certain image pair in the benchmark. Note to put a slash / at the end the folder name. See the given example."<<endl;
		cout<<"(P3): the first image."<<endl;
		cout<<"(P4): the second image."<<endl;
		cout<<"(P5): the file containing the extracted line segment from the first image."<<endl;
		cout<<"(P6): the file containing the extracted line segment from the second image."<<endl;
		cout<<"(P7): the selected line segment detector, must be either LSD or EDLines."<<endl;
		cout<<"(P8): the file containing the matches you obtained."<<endl;		
	}

	string folderName = argv[1];
	string imgName1 = folderName+argv[2];
	string imgName2 = folderName+argv[3];
	string lineName1 = folderName+argv[4];
	string lineName2 = folderName+argv[5];
	string detectorName = argv[6];	

	string groundTruthName;
	if (detectorName.compare("LSD")==0)
		groundTruthName = "lsd_GroundTruthMatches.txt";
	else if (detectorName.compare("EDLines")==0)
		groundTruthName = "ed_GroundTruthMatches.txt";
	else
		cout<<"line segment detector must be either LSD or EDLines!";
	
	groundTruthName = folderName + groundTruthName;
	string obtainedMatchName = argv[7];
	
	Mat img1= imread(imgName1, 3);    
	Mat img2= imread(imgName2, 3);    	
	OpenCVMatrixIO<float> *pFltIO = new OpenCVMatrixIO<float>();	
	Mat detectedLines1, detectedLines2;
	pFltIO->readDataToMat(lineName1, detectedLines1, 4);
	pFltIO->readDataToMat(lineName2, detectedLines2, 4);		
	vector<vector<int>> gvec1, gvec2;
	int ntotal = readGroundTruth(groundTruthName, gvec1, gvec2);
	delete pFltIO; pFltIO = NULL;

	Mat obtainedMatches;
	OpenCVMatrixIO<int> *pIntIO = new OpenCVMatrixIO<int>();	
	pIntIO->readDataToMat(obtainedMatchName, obtainedMatches, 2);
	int nobtain = obtainedMatches.rows;

	vector<bool> vaccess(nobtain,0);
	int nTotalCorrect = resultEvaluation(obtainedMatches, gvec1, gvec2, vaccess);

	float recall = ((double)nTotalCorrect) / ntotal;
	float accuracy = ((double)nTotalCorrect) / obtainedMatches.rows;	

	string reportName = folderName+"report.txt";
	string visualReportName = folderName+"visualized report.jpg";
	ofstream outFile(reportName.c_str(), ios_base::out);  
	outFile<<"Line segments detected in the two images: ("<<detectedLines1.rows<<","<<detectedLines2.rows<<")"<<endl;
	outFile<<"Ground truth matches among the detected line segments: "<<ntotal<<endl;
	outFile<<"The total number of matches found: "<<nobtain<<endl;
	outFile<<"The number of correct matches among the total matches: "<<nTotalCorrect<<endl;
	outFile<<"Accuracy: "<<accuracy<<endl;
	outFile<<"Recall: "<<recall<<endl;
	outFile<<endl;
	outFile<<"Correctness accessment of the obtained matches ('(0)' marks correct matches, '(1)' marks incorrect ones):"<<endl;
	for(int i = 0; i < nobtain; i++)
		outFile<<"("<<vaccess[i]<<") "<<obtainedMatches.at<int>(i,0)<<"\t"<<obtainedMatches.at<int>(i,1)<<endl;	
	outFile.close();
	//	nobtain<<"\t"<<nTotalCorrect<<"\t"<<nobtain-nTotalCorrect<<"\t"<<ntotal<<"\t"<<recall<<"\t"<<precision<<endl;
	visualizeResultReport(img1, img2, detectedLines1, detectedLines2, obtainedMatches, vaccess, gvec1, gvec2, visualReportName);

	return 1;
}