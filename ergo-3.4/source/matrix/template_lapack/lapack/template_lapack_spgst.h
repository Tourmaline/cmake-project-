/* Ergo, version 3.4, a program for linear scaling electronic structure
 * calculations.
 * Copyright (C) 2014 Elias Rudberg, Emanuel H. Rubensson, and Pawel Salek.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Primary academic reference:
 * Kohn−Sham Density Functional Theory Electronic Structure Calculations 
 * with Linearly Scaling Computational Time and Memory Usage,
 * Elias Rudberg, Emanuel H. Rubensson, and Pawel Salek,
 * J. Chem. Theory Comput. 7, 340 (2011),
 * <http://dx.doi.org/10.1021/ct100611z>
 * 
 * For further information about Ergo, see <http://www.ergoscf.org>.
 */
 
 /* This file belongs to the template_lapack part of the Ergo source 
  * code. The source files in the template_lapack directory are modified
  * versions of files originally distributed as CLAPACK, see the
  * Copyright/license notice in the file template_lapack/COPYING.
  */
 

#ifndef TEMPLATE_LAPACK_SPGST_HEADER
#define TEMPLATE_LAPACK_SPGST_HEADER

#include "template_lapack_common.h"

template<class Treal>
int template_lapack_spgst(const integer *itype, const char *uplo, const integer *n, 
	Treal *ap, const Treal *bp, integer *info)
{
/*  -- LAPACK routine (version 3.0) --   
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,   
       Courant Institute, Argonne National Lab, and Rice University   
       March 31, 1993   


    Purpose   
    =======   

    DSPGST reduces a real symmetric-definite generalized eigenproblem   
    to standard form, using packed storage.   

    If ITYPE = 1, the problem is A*x = lambda*B*x,   
    and A is overwritten by inv(U**T)*A*inv(U) or inv(L)*A*inv(L**T)   

    If ITYPE = 2 or 3, the problem is A*B*x = lambda*x or   
    B*A*x = lambda*x, and A is overwritten by U*A*U**T or L**T*A*L.   

    B must have been previously factorized as U**T*U or L*L**T by DPPTRF.   

    Arguments   
    =========   

    ITYPE   (input) INTEGER   
            = 1: compute inv(U**T)*A*inv(U) or inv(L)*A*inv(L**T);   
            = 2 or 3: compute U*A*U**T or L**T*A*L.   

    UPLO    (input) CHARACTER   
            = 'U':  Upper triangle of A is stored and B is factored as   
                    U**T*U;   
            = 'L':  Lower triangle of A is stored and B is factored as   
                    L*L**T.   

    N       (input) INTEGER   
            The order of the matrices A and B.  N >= 0.   

    AP      (input/output) DOUBLE PRECISION array, dimension (N*(N+1)/2)   
            On entry, the upper or lower triangle of the symmetric matrix   
            A, packed columnwise in a linear array.  The j-th column of A   
            is stored in the array AP as follows:   
            if UPLO = 'U', AP(i + (j-1)*j/2) = A(i,j) for 1<=i<=j;   
            if UPLO = 'L', AP(i + (j-1)*(2n-j)/2) = A(i,j) for j<=i<=n.   

            On exit, if INFO = 0, the transformed matrix, stored in the   
            same format as A.   

    BP      (input) DOUBLE PRECISION array, dimension (N*(N+1)/2)   
            The triangular factor from the Cholesky factorization of B,   
            stored in the same format as A, as returned by DPPTRF.   

    INFO    (output) INTEGER   
            = 0:  successful exit   
            < 0:  if INFO = -i, the i-th argument had an illegal value   

    =====================================================================   


       Test the input parameters.   

       Parameter adjustments */
    /* Table of constant values */
     integer c__1 = 1;
     Treal c_b9 = -1.;
     Treal c_b11 = 1.;
    
    /* System generated locals */
    integer i__1, i__2;
    Treal d__1;
    /* Local variables */
     integer j, k;
     logical upper;
     integer j1, k1;
     integer jj, kk;
     Treal ct;
     Treal ajj;
     integer j1j1;
     Treal akk;
     integer k1k1;
     Treal bjj, bkk;


    --bp;
    --ap;

    /* Function Body */
    *info = 0;
    upper = template_blas_lsame(uplo, "U");
    if (*itype < 1 || *itype > 3) {
	*info = -1;
    } else if (! upper && ! template_blas_lsame(uplo, "L")) {
	*info = -2;
    } else if (*n < 0) {
	*info = -3;
    }
    if (*info != 0) {
	i__1 = -(*info);
	template_blas_erbla("SPGST ", &i__1);
	return 0;
    }

    if (*itype == 1) {
	if (upper) {

/*           Compute inv(U')*A*inv(U)   

             J1 and JJ are the indices of A(1,j) and A(j,j) */

	    jj = 0;
	    i__1 = *n;
	    for (j = 1; j <= i__1; ++j) {
		j1 = jj + 1;
		jj += j;

/*              Compute the j-th column of the upper triangle of A */

		bjj = bp[jj];
		template_blas_tpsv(uplo, "Transpose", "Nonunit", &j, &bp[1], &ap[j1], &
			c__1);
		i__2 = j - 1;
		template_blas_spmv(uplo, &i__2, &c_b9, &ap[1], &bp[j1], &c__1, &c_b11, &
			ap[j1], &c__1);
		i__2 = j - 1;
		d__1 = 1. / bjj;
		template_blas_scal(&i__2, &d__1, &ap[j1], &c__1);
		i__2 = j - 1;
		ap[jj] = (ap[jj] - template_blas_dot(&i__2, &ap[j1], &c__1, &bp[j1], &
			c__1)) / bjj;
/* L10: */
	    }
	} else {

/*           Compute inv(L)*A*inv(L')   

             KK and K1K1 are the indices of A(k,k) and A(k+1,k+1) */

	    kk = 1;
	    i__1 = *n;
	    for (k = 1; k <= i__1; ++k) {
		k1k1 = kk + *n - k + 1;

/*              Update the lower triangle of A(k:n,k:n) */

		akk = ap[kk];
		bkk = bp[kk];
/* Computing 2nd power */
		d__1 = bkk;
		akk /= d__1 * d__1;
		ap[kk] = akk;
		if (k < *n) {
		    i__2 = *n - k;
		    d__1 = 1. / bkk;
		    template_blas_scal(&i__2, &d__1, &ap[kk + 1], &c__1);
		    ct = akk * -.5;
		    i__2 = *n - k;
		    template_blas_axpy(&i__2, &ct, &bp[kk + 1], &c__1, &ap[kk + 1], &c__1)
			    ;
		    i__2 = *n - k;
		    template_blas_spr2(uplo, &i__2, &c_b9, &ap[kk + 1], &c__1, &bp[kk + 1]
			    , &c__1, &ap[k1k1]);
		    i__2 = *n - k;
		    template_blas_axpy(&i__2, &ct, &bp[kk + 1], &c__1, &ap[kk + 1], &c__1)
			    ;
		    i__2 = *n - k;
		    template_blas_tpsv(uplo, "No transpose", "Non-unit", &i__2, &bp[k1k1],
			     &ap[kk + 1], &c__1);
		}
		kk = k1k1;
/* L20: */
	    }
	}
    } else {
	if (upper) {

/*           Compute U*A*U'   

             K1 and KK are the indices of A(1,k) and A(k,k) */

	    kk = 0;
	    i__1 = *n;
	    for (k = 1; k <= i__1; ++k) {
		k1 = kk + 1;
		kk += k;

/*              Update the upper triangle of A(1:k,1:k) */

		akk = ap[kk];
		bkk = bp[kk];
		i__2 = k - 1;
		template_blas_tpmv(uplo, "No transpose", "Non-unit", &i__2, &bp[1], &ap[
			k1], &c__1);
		ct = akk * .5;
		i__2 = k - 1;
		template_blas_axpy(&i__2, &ct, &bp[k1], &c__1, &ap[k1], &c__1);
		i__2 = k - 1;
		template_blas_spr2(uplo, &i__2, &c_b11, &ap[k1], &c__1, &bp[k1], &c__1, &
			ap[1]);
		i__2 = k - 1;
		template_blas_axpy(&i__2, &ct, &bp[k1], &c__1, &ap[k1], &c__1);
		i__2 = k - 1;
		template_blas_scal(&i__2, &bkk, &ap[k1], &c__1);
/* Computing 2nd power */
		d__1 = bkk;
		ap[kk] = akk * (d__1 * d__1);
/* L30: */
	    }
	} else {

/*           Compute L'*A*L   

             JJ and J1J1 are the indices of A(j,j) and A(j+1,j+1) */

	    jj = 1;
	    i__1 = *n;
	    for (j = 1; j <= i__1; ++j) {
		j1j1 = jj + *n - j + 1;

/*              Compute the j-th column of the lower triangle of A */

		ajj = ap[jj];
		bjj = bp[jj];
		i__2 = *n - j;
		ap[jj] = ajj * bjj + template_blas_dot(&i__2, &ap[jj + 1], &c__1, &bp[jj 
			+ 1], &c__1);
		i__2 = *n - j;
		template_blas_scal(&i__2, &bjj, &ap[jj + 1], &c__1);
		i__2 = *n - j;
		template_blas_spmv(uplo, &i__2, &c_b11, &ap[j1j1], &bp[jj + 1], &c__1, &
			c_b11, &ap[jj + 1], &c__1);
		i__2 = *n - j + 1;
		template_blas_tpmv(uplo, "Transpose", "Non-unit", &i__2, &bp[jj], &ap[jj],
			 &c__1);
		jj = j1j1;
/* L40: */
	    }
	}
    }
    return 0;

/*     End of DSPGST */

} /* dspgst_ */

#endif
