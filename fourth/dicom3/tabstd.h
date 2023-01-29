/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/* tabstd.h 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved. */

#ifndef _DC3tabstd_HEADER_
#define _DC3tabstd_HEADER_

#include "tabst.h"

static struct DC3AbstractSyntaxTableEntry {
	char *			UID;
	char *			Description;
	unsigned		Supported;
} DC3AbstractSyntaxTable[] =
{
	DC3VerificationSOPClass,			"Verification SOP Class", 1,
	DC3MediaStorageDirectoryStorageSOPClass,	"Media Storage Directory Storage SOP Class", 1,
	DC3BasicStudyContentNotificationSOPClass, 	"Basic Study Content Notification SOP Class", 	0,

	DC3StorageCommitmentPushModelSOPClass, 		"Storage Commitment Push Model SOP Class", 	0,
	DC3StorageCommitmentPushModelSOPInstance, 	"Storage Commitment Push Model SOP Instance", 	0,
	DC3StorageCommitmentPullModelSOPClass, 		"Storage Commitment Pull Model SOP Class", 	0,
	DC3StorageCommitmentPullModelSOPInstance, 	"Storage Commitment Pull Model SOP Instance", 	0,

	DC3DetachedPatientManagementSOPClass, 		"Detached Patient Management SOP Class", 	0,
	DC3DetachedPatientManagementMetaSOPClass, 	"Detached Patient Management Meta SOP Class",	0,
	DC3DetachedVisitManagementSOPClass, 		"Detached Visit Management SOP Class", 		0,
	DC3DetachedStudyManagementSOPClass, 		"Detached Study Management SOP Class", 		0,
	DC3StudyComponentManagementSOPClass, 		"Study Component Management SOP Class", 	0,
	DC3DetachedResultsManagementSOPClass, 		"Detached Results Management SOP Class", 	0,
	DC3DetachedResultsManagementMetaSOPClass, 	"Detached Results Management Meta SOP Class", 	0,
	DC3DetachedStudyManagementMetaSOPClass, 	"Detached Study Management Meta SOP Class", 	0,
	DC3DetachedInterpretationManagementSOPClass, 	"Detached Interpretation Management SOP Class",	0,

	DC3BasicFilmSessionSOPClass,				"Basic Film Session SOP Class", 			0,
	DC3BasicFilmBoxSOPClass,				"Basic Film Box SOP Class", 				0,
	DC3BasicGrayscaleImageBoxSOPClass,			"Basic Grayscale Image Box SOP Class", 			0,
	DC3BasicColorImageBoxSOPClass,				"Basic Color Image Box SOP Class", 			0,
	DC3ReferencedImageBoxSOPClass,				"Referenced Image Box SOP Class", 			0,
	DC3BasicGrayscalePrintManagementMetaSOPClass,		"Basic Grayscale Print Management Meta SOP Class",	0,
	DC3ReferencedGrayscalePrintManagementMetaSOPClass,	"Referenced Grayscale  PrintManagement SOP Class",	0,
	DC3PrintJobSOPClass,					"Print Job SOP Class", 					0,
	DC3BasicAnnotationBoxSOPClass,				"Basic Annotation Box SOP Class", 			0,
	DC3PrinterSOPClass,					"Printer SOP Class", 					0,
	DC3BasicColorPrintManagementMetaSOPClass,		"Basic Color Print Management Meta SOP Class", 		0,
	DC3ReferencedColorPrintManagementMetaSOPClass,		"Referenced Color Print Management Meta SOP Class", 	0,
	DC3VOILUTBoxSOPClass,					"VOI LUT Box SOP Class", 				0,
	DC3ImageOverlayBoxSOPClass,				"Image Overlay Box SOP Class", 				0,

	DC3ComputedRadiographyImageStorageSOPClass,		"Computed Radiography Image Storage SOP Class",		0,
	DC3CTImageStorageSOPClass,				"CT Image Storage SOP Class",				1,
	DC3UltrasoundMultiframeImageStorageSOPClass,		"Ultrasound Multiframe Image Storage SOP Class",	0,
	DC3MRImageStorageSOPClass,				"MR Image Storage SOP Class",				1,
	DC3NuclearMedicineImageStorageSOPClass,			"Nuclear Medicine Image Storage SOP Class",		0,
	DC3UltrasoundImageStorageSOPClass,			"Ultrasound ImageS torage SOP Class",			0,
	DC3SecondaryCaptureImageStorageSOPClass,		"Secondary Capture Image Storage SOP Class",		0,
	DC3StandaloneOverlayStorageSOPClass,			"Standalone Overlay Storage SOP Class",			0,
	DC3StandaloneCurveStorageSOPClass,			"Standalone CurveStorage SOP Class",			0,
	DC3StandaloneModalityLUTStorageSOPClass,		"Standalone Modality LUT Storage SOP Class",		0,
	DC3StandaloneVOILUTStorageSOPClass,			"Standalone VOI LUT Storage SOP Class",			0,
	DC3XrayAngiographicImageStorageSOPClass,		"X-ray Angiographic Image Storage SOP Class",		0,
	DC3XrayRadioFlouroscopicImageStorageSOPClass,		"X-ray RadioFlouroscopic Image Storage SOP Class",	0,
	DC3XrayAngiographicBiplaneImageStorageSOPClass,		"X-ray Angiographic Bi-plane Image Storage SOP Class",	0,

	DC3PatientRootQueryRetrieveInformationModelFindSOPClass,	"Patient Root Query/Retrieve Information Model - Find SOP Class",	0,
	DC3PatientRootQueryRetrieveInformationModelMoveSOPClass,	"Patient Root Query/Retrieve Information Model - Move SOP Class",	0,
	DC3PatientRootQueryRetrieveInformationModelGetSOPClass,		"Patient Root Query/Retrieve Information Model - Get SOP Class",	0,
	DC3StudyRootQueryRetrieveInformationModelFindSOPClass,		"Study Root Query/Retrieve Information Model - Find SOP Class",		0,
	DC3StudyRootQueryRetrieveInformationModelMoveSOPClass,		"Study Root Query/Retrieve Information Model - Move SOP Class",		0,
	DC3StudyRootQueryRetrieveInformationModelGetSOPClass,		"Study Root Query/Retrieve Information Model - Get SOP Class",		0,
	DC3PatientStudyOnlyQueryRetrieveInformationModelFindSOPClass,	"Patient/Study Only Query/Retrieve Information Model - Find SOP Class",	0,
	DC3PatientStudyOnlyQueryRetrieveInformationModelMoveSOPClass,	"Patient/Study Only Query/Retrieve Information Model - Move SOP Class",	0,
	DC3PatientStudyOnlyQueryRetrieveInformationModelGetSOPClass,	"Patient/Study Only Query/Retrieve Information Model - Get SOP Class",	0,

	DC3ModalityWorklistInformationModelFindSOPClass,	"Modality Worklist Information Model - Find SOP Class",	0,

	0,0,0
};


#endif /* _DC3tabstd_HEADER_ */





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
