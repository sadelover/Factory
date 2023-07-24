
/*
	This header file refers to the table uppc_libmodel.imageproperty

*/

#ifndef DEFINE_IMAGEPROPERTY_h
#define DEFINE_IMAGEPROPERTY_h

typedef struct tag_ImageProperty
{
	int iID;		// id in the picture lib
	int iType;		// type of the picture.
	int iState;		// the state of the equipment.
	int IRegion;	// the region of the state in the image.	
}ImagePropertyEntry, *pImagePropertyEntry;

enum IMAGEPROPERTY_COLUMN
{
	IMAGEPROPERTY_ID,
	IMAGEPROPERTY_TYPE,
	IMAGEPROPERTY_STATE,
	IMAGEPROPERTY_REGION,
	IMAGEPROPERTY_MAXNUM,
};


#endif


