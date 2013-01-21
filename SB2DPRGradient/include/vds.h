/***************************************************************************
 
 Copyright 2011 HeartVista Inc.  All rights reserved.
 Contact: HeartVista, Inc. <rthawk-info@heartvista.com>
 
 This file is part of the RTHawk system.
 
 $HEARTVISTA_BEGIN_LICENSE$
 
 THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF HEARTVISTA
 The copyright notice above does not evidence any	
 actual or intended publication of such source code.
 
 $HEARTVISTA_END_LICENSE$
 
 ***************************************************************************/

void calc_vds(const double slewmax, const double gradmax, const double gamma, const double Tgsample, const double Tdsample,
              const int Ninterleaves, const double * const fov, const int numfov, const double krmax,
              const int ngmax, double **xgrad, double**ygrad,
              int *numgrad, double* effectiveKMax);
