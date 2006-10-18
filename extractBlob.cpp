/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
// Blobs
#include <Blob.h>
#include <BlobResult.h>
#include "videotracker.h"

struct coordinate* extractBlob(IplImage* tmp_frame, IplImage* background){
    IplImage* subbedImg = cvCloneImage(tmp_frame);
	getFiltredBinaryImage(tmp_frame,background,2);
	 //cvSub( tmp_frame, background, subbedImg, NULL );
	//if(!cvSaveImage("subbed.jpg",subbedImg)) printf("Could not save the backgroundimage\n");
	
	IplImage* img = cvCreateImage(cvGetSize(subbedImg),IPL_DEPTH_8U,1);
	//IplImage* binImg = cvCreateImage(cvGetSize(subbedImg),IPL_DEPTH_8U,1);
	
	cvCvtColor(subbedImg, img, CV_RGB2GRAY);

	//cvAdaptiveThreshold( img, binImg,CV_THRESH_BINARY_INV,CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV,3, 5 );
	
	// FIND AND MARK THE BLOBS
	// delcare a set of blob results
	CBlobResult blobs;
	// get the blobs from the image, with no mask, using a threshold of 100
	blobs = CBlobResult( img, NULL, 10, true );
	
	blobs.PrintBlobs( "blobs.txt" );

	// discard the blobs with less area than 5000 pixels
	// ( the criteria to filter can be any class derived from COperadorBlob )
	blobs.Filter( blobs, B_INCLUDE, CBlobGetArea(), B_GREATER, 100 );
	//queste due righe sono per filtrare i 2 blob del centro dell' immagine
	blobs.Filter( blobs, B_INCLUDE, CBlobGetArea(), B_LESS, (img->height)*(img->width)*0.8);
	blobs.Filter( blobs, B_INCLUDE, CBlobGetPerimeter(), B_LESS, (img->height)+(img->width)*2*0.8);
	// create a file with filtered results
	blobs.PrintBlobs( "filteredBlobs.txt" );

	// mark the blobs on the image
	int i;
	// delare a single blob
	CBlob Blob;
	// some vars
	int iMaxx, iMinx, iMaxy, iMiny, iMeanx, iMeany;
	// for each blob
	for  (i=0; i<blobs.GetNumBlobs(); ++i)
	{
		// get the blob info
		Blob = blobs.GetBlob(i);
		// get max, and min co-ordinates
		iMaxx=Blob.MaxX();
		iMinx=Blob.MinX();
		iMaxy=Blob.MaxY();
		iMiny=Blob.MinY();
		// find the average of the blob (i.e. estimate its centre)
		iMeanx=(iMinx+iMaxx)/2;
		iMeany=(iMiny+iMaxy)/2;
		// mark centre
		cvLine( img, cvPoint(iMeanx, iMeany), cvPoint(iMeanx, iMeany), CV_RGB(255, 255 , 255), 4, 8, 0 );
		// mark box around blob
		cvRectangle( img, cvPoint(iMinx , iMiny ), cvPoint ( iMaxx, iMaxy ), CV_RGB(255, 255 , 255), 1, 8, 0);
		// print the blob centres
		printf("\n%d, X: %d, Y: %d\n", i, iMeanx, iMeany);
	}// each blob

	// display the image
	//cvNamedWindow("image",1);
	//cvShowImage("image", img);
	// keep image 'til keypress
	//cvWaitKey(0);
	// release the image
	cvReleaseImage(&img);
	
	return 0;
}




IplImage* getFiltredBinaryImage(IplImage* currentImage, IplImage* backgroundImage, int value ){
	
	int height,width,step,channels;
    uchar * dataCurrentImage;
	uchar* dataBackgroundImage;
	uchar* dataBinImage;
    int i,j,k;
	
	//if(!cvSaveImage("current.jpg",currentImage)) printf("Could not save the backgroundimage\n");
	IplImage* img = cvCreateImage(cvGetSize(currentImage),IPL_DEPTH_8U,1);
	IplImage* binImg = cvCreateImage(cvGetSize(currentImage),IPL_DEPTH_8U,1);
	cvCvtColor(currentImage, img, CV_RGB2GRAY);
	//if(!cvSaveImage("img.jpg",img)) printf("Could not save the backgroundimage\n");
	cvAdaptiveThreshold(img,binImg,CV_THRESH_BINARY_INV,CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV,3, 5 );
	
	//if(!cvSaveImage("binaria.jpg",binImg)) printf("Could not save the backgroundimage\n");
	
	cvReleaseImage(&img);
	
	height    = currentImage->height;
	width     = currentImage->width;
	step      = currentImage->widthStep;
	channels  = currentImage->nChannels;
	dataCurrentImage      = (uchar *) currentImage->imageData;
	dataBackgroundImage   = (uchar *) backgroundImage->imageData;
	dataBinImage   = (uchar *) binImg->imageData;

	for(i=0;i<height;i++) for(j=0;j<width;j++) for(k=0;k<channels;k++)
    if ( dataCurrentImage[i*step+j*channels+k] - dataBackgroundImage[i*step+j*channels+k] >= value )
		dataBinImage[i*step+j*channels+k] = 1;
	
	else  dataBinImage[i*step+j*channels+k] = 0;
	
	}

















