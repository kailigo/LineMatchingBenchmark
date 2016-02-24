#ifndef _OPEN_CV_MATRIX_IO_H_
#define _OPEN_CV_MATRIX_IO_H_

#include <iostream>
#include <fstream> 
#include "cv.h"
#include "stdio.h"
#include <iterator>
//#include "OpencvDataConversion.h"

using namespace cv;
using namespace std;

template<class DataType>
class OpenCVMatrixIO
{
public:

	OpenCVMatrixIO(){};
	~OpenCVMatrixIO(){};

	static bool readDataToMat(const string fileName, Mat &matData, int matCols=0, int matRows=0, int matChns=1)
	{
		ifstream inFile(fileName.c_str(), ios_base::in);  
		if(!inFile.is_open())  
			return 0; 
		
		std::istream_iterator<DataType> begin(inFile);   
		std::istream_iterator<DataType> end;          
		vector<DataType> inData(begin,end);      
		size_t dataLength = inData.size();		

		if (matChns == 0)  matChns = 1;  			
		if ( ! matCols )
		{
			matRows = inData[0];
			dataLength--;  
			inData.erase(inData.begin());
		}
		else
		{
			matRows = dataLength/matCols;
		}
		cv::Mat tmpMat = cv::Mat(inData);       
		matData = tmpMat.reshape(matChns, matRows).clone();  

		return 1;  
	}

	static int writeMatData2File(string fileName, cv::Mat& matData)  
	{  
		int retVal = 0;  
		if (matData.empty())  
		{  
			cout << "empty matrix" << endl;   
			retVal = 1;  
			return (retVal);  
		}  

		std::ofstream outFile(fileName.c_str(), ios_base::out);  
		if (!outFile.is_open())  
		{  
			cout << "fail to open the file" << endl;   
			retVal = -1;  
			return (retVal);  
		}  

		int num = 0;
		for (int r = 0; r < matData.rows; r++)  
		{  
			for (int c = 0; c < matData.cols; c++)  
			{  
				DataType data = matData.at<DataType>(r,c);   
				outFile<< data << "\t" ;   
			}  			
			outFile << endl;  
		}  
		return (retVal);  
	}  	
};

#endif  // _OPEN_CV_MATRIX_IO_H_