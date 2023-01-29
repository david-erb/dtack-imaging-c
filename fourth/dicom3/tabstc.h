/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/* tabstc.h 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved. */

#ifndef _DC3tabstc_HEADER_
#define _DC3tabstc_HEADER_

#define	DC3VerificationSOPClass					"1.2.840.10008.1.1"
#define	DC3MediaStorageDirectoryStorageSOPClass			"1.2.840.10008.1.3.10"
#define	DC3BasicStudyContentNotificationSOPClass		"1.2.840.10008.1.9"
#define	DC3StorageCommitmentPushModelSOPClass			"1.2.840.10008.1.20.1"
#define	DC3StorageCommitmentPushModelSOPInstance		"1.2.840.10008.1.20.1.1"
#define	DC3StorageCommitmentPullModelSOPClass			"1.2.840.10008.1.20.2"
#define	DC3StorageCommitmentPullModelSOPInstance		"1.2.840.10008.1.20.2.1"

#define	DC3DetachedPatientManagementSOPClass			"1.2.840.10008.3.1.2.1.1"
#define	DC3DetachedPatientManagementMetaSOPClass		"1.2.840.10008.3.1.2.1.4"
#define	DC3DetachedVisitManagementSOPClass			"1.2.840.10008.3.1.2.2.1"
#define	DC3DetachedStudyManagementSOPClass			"1.2.840.10008.3.1.2.3.1"
#define	DC3StudyComponentManagementSOPClass			"1.2.840.10008.3.1.2.3.2"
#define	DC3DetachedResultsManagementSOPClass			"1.2.840.10008.3.1.2.5.1"
#define	DC3DetachedResultsManagementMetaSOPClass		"1.2.840.10008.3.1.2.5.4"
#define	DC3DetachedStudyManagementMetaSOPClass			"1.2.840.10008.3.1.2.5.5"
#define	DC3DetachedInterpretationManagementSOPClass		"1.2.840.10008.3.1.2.6.1"

#define	DC3BasicFilmSessionSOPClass				"1.2.840.10008.5.1.1.1"
#define	DC3BasicFilmBoxSOPClass					"1.2.840.10008.5.1.1.2"
#define	DC3BasicGrayscaleImageBoxSOPClass			"1.2.840.10008.5.1.1.4"
#define	DC3BasicColorImageBoxSOPClass				"1.2.840.10008.5.1.1.4.1"
#define	DC3ReferencedImageBoxSOPClass				"1.2.840.10008.5.1.1.4.2"
#define	DC3BasicGrayscalePrintManagementMetaSOPClass		"1.2.840.10008.5.1.1.9"
#define	DC3ReferencedGrayscalePrintManagementMetaSOPClass	"1.2.840.10008.5.1.1.9.1"
#define	DC3PrintJobSOPClass					"1.2.840.10008.5.1.1.14"
#define	DC3BasicAnnotationBoxSOPClass				"1.2.840.10008.5.1.1.15"
#define	DC3PrinterSOPClass					"1.2.840.10008.5.1.1.16"
#define	DC3BasicColorPrintManagementMetaSOPClass		"1.2.840.10008.5.1.1.18"
#define	DC3ReferencedColorPrintManagementMetaSOPClass		"1.2.840.10008.5.1.1.18.1"
#define	DC3VOILUTBoxSOPClass					"1.2.840.10008.5.1.1.22"
#define	DC3ImageOverlayBoxSOPClass				"1.2.840.10008.5.1.1.24"

#define	DC3ComputedRadiographyImageStorageSOPClass		"1.2.840.10008.5.1.4.1.1.1"
#define	DC3CTImageStorageSOPClass				"1.2.840.10008.5.1.4.1.1.2"
#define	DC3UltrasoundMultiframeImageStorageRetiredSOPClass	"1.2.840.10008.5.1.4.1.1.3"
#define	DC3UltrasoundMultiframeImageStorageSOPClass		"1.2.840.10008.5.1.4.1.1.3.1"
#define	DC3MRImageStorageSOPClass				"1.2.840.10008.5.1.4.1.1.4"
#define	DC3NuclearMedicineImageStorageSOPClass			"1.2.840.10008.5.1.4.1.1.5"
#define	DC3UltrasoundImageStorageRetiredSOPClass		"1.2.840.10008.5.1.4.1.1.6"
#define	DC3UltrasoundImageStorageSOPClass			"1.2.840.10008.5.1.4.1.1.6.1"
#define	DC3SecondaryCaptureImageStorageSOPClass			"1.2.840.10008.5.1.4.1.1.7"
#define	DC3StandaloneOverlayStorageSOPClass			"1.2.840.10008.5.1.4.1.1.8"
#define	DC3StandaloneCurveStorageSOPClass			"1.2.840.10008.5.1.4.1.1.9"
#define	DC3StandaloneModalityLUTStorageSOPClass			"1.2.840.10008.5.1.4.1.1.10"
#define	DC3StandaloneVOILUTStorageSOPClass			"1.2.840.10008.5.1.4.1.1.11"
#define	DC3XrayAngiographicImageStorageSOPClass			"1.2.840.10008.5.1.4.1.1.12.1"
#define	DC3XrayRadioFlouroscopicImageStorageSOPClass		"1.2.840.10008.5.1.4.1.1.12.2"
#define	DC3XrayAngiographicBiplaneImageStorageSOPClass		"1.2.840.10008.5.1.4.1.1.12.3"

#define	DC3PatientRootQueryRetrieveInformationModelFindSOPClass		"1.2.840.10008.5.1.4.1.2.1.1"
#define	DC3PatientRootQueryRetrieveInformationModelMoveSOPClass		"1.2.840.10008.5.1.4.1.2.1.2"
#define	DC3PatientRootQueryRetrieveInformationModelGetSOPClass		"1.2.840.10008.5.1.4.1.2.1.3"
#define	DC3StudyRootQueryRetrieveInformationModelFindSOPClass		"1.2.840.10008.5.1.4.1.2.2.1"
#define	DC3StudyRootQueryRetrieveInformationModelMoveSOPClass		"1.2.840.10008.5.1.4.1.2.2.2"
#define	DC3StudyRootQueryRetrieveInformationModelGetSOPClass		"1.2.840.10008.5.1.4.1.2.2.3"
#define	DC3PatientStudyOnlyQueryRetrieveInformationModelFindSOPClass	"1.2.840.10008.5.1.4.1.2.3.1"
#define	DC3PatientStudyOnlyQueryRetrieveInformationModelMoveSOPClass	"1.2.840.10008.5.1.4.1.2.3.2"
#define	DC3PatientStudyOnlyQueryRetrieveInformationModelGetSOPClass	"1.2.840.10008.5.1.4.1.2.3.3"

#define	DC3ModalityWorklistInformationModelFindSOPClass			"1.2.840.10008.5.1.4.31"

#endif /* _DC3tabstc_HEADER_ */





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
