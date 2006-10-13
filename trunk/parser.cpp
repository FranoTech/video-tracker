#include "videotracker.h"

/****************************************************************************
This function permits to read the file that contains the data for the matrices
of the Kalman Filter
*****************************************************************************/

//ValuesVect contiene tutti i dati contenuti nelle matrici
//MDSC contiene i dati che descrivono le matrici - necessario per la ricostruzione


void parse(char * fileName,std::vector<float>* ValuesVect,struct matrixDesc* MDSC){
 	wxString Stringa;
	const wxChar* perm = _("r");
	const wxChar* newFileName=_("./data.txt"); //it mus be not written,but a filename passed in the char* fileName variable
	wxFFile file(newFileName, perm);
	file.ReadAll(&Stringa);//put the content of the file in a string

	int nMatrix = Stringa.Freq(']');//# matrices in data file

	//MDSC = new struct matrixDesc[nMatrix];
	//std::vector<float> ValuesVect;
	
	
	for (int i=0;i<nMatrix;i++){
		wxString Matrix = Stringa.AfterFirst('[');
		Matrix = Matrix.BeforeFirst(']');
		int s = Matrix.size();
		Stringa.Remove(0,s+2);
		
		Matrix += ';';
		int nRows = Matrix.Freq(';');//# rows of the matrix
		MDSC[i].nRows=nRows;
		for (int j=0;j<nRows;j++){
			wxString Row = Matrix.BeforeFirst(';');
			int z;
			z = Row.size();
			Matrix.Remove(0,z+1);
			
			Row +=(',');
			int nCols = Row.Freq(',');
			MDSC[i].nCols=nCols;//# cols of the matrix
			//get data
			for (int h=0;h<nCols;h++){
				wxString Col = Row.BeforeFirst(',');
				const char* col = (const char *) Col.c_str(); // modified to be compiled under linux
				float val = strtod(col,NULL);
				ValuesVect->push_back(val);//put the data in the vector

				int p;
				p = Col.size();
				Row.Remove(0,p+1);

				}
			}
		}	

}