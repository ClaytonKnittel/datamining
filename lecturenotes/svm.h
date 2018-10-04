//
//  svm.h
//  datamining
//
//  Created by Clayton Knittel on 10/2/18.
//  Copyright © 2018 Clayton Knittel. All rights reserved.
//

#ifndef vsm_h
#define vsm_h


/*
 
 Map input space to a feature space of higher or equal dimension.
 Ideally this will make the separation of categories linear.
 
 Use the dot product as a similarity measure.
 
 Simple example:
 
 ---0-0-0---x-x-x----
 Simple, separate here
 ---0-0-0-|-x-x-x----
 
 What about
 -x-0-0-x-0-x-x-0---
 then try
 
 -x-----x---x-x-----
 ---0-0---0-----0---
 
 Now linearly separable.
 
 
 
 SVM:
 
 Given D = {{xi, yi}},    yi in {0, 1} (binary)
 find y = f(x)
 
 
 Simple case:
 mapping v = phi(x) = x, identity mapping
 input space = feature space
 
 Separation must maximize the margin from all the data
 points to the separation, yet still separates them.
 
 
 D+/- = {{vi, yi} | yi = +/-}
 
 In order to determine how close points are to the separation,
 compute "center of mass" of the different sets of outputs
 C+ and C-;
 
 C+ = 1/n+ total[v in D+]
 C- = 1/n- total[v in D-]
 
 W = C+ - C-
 C = (C+ + C-) / 2
 
 Separating line is set of all vectors such that
 (v - W).C = 0,
 v.C = W.C
 
 y = sign(W.(x - C)) = sign(v - ((C+ + C-) / 2).(C+ - C-))
 = sign((v.V+) - (v.C-) + b) = sign(v.W - W.C)
 = sign(1/n+ total(v.vi in +) - 1/n- total(v.vi in -) + b)
 
 b = 1/2(|C-|^2 - |C+|^2) = [-W.C]
 
 Generally:
 y = sign(1/n+ total(phi(x).phi(xi) in +) - 1/n- total(phi(x).phi(xi) in -) + b)
 
 
 But how do we know this mapping?
 
 Kernel trick:
 k(x, x') = (phi(x), phi(x')) = (v, v')
 k(x, x') similarity in the input space.
 
 k is kernel function = similarity measure in
 input space.
 
 This trick says that the similarity in the
 input space is the same as in the feature space.
 In other words, you are preserving similarity in
 transforming.
 
 Thus,
 y = sign(1/n+ total(k(x.xi) in +) - 1/n- total(k(x.xi) in -) + b)
 
 
 
 Margin related to 1 / |W|
 |W| is normalization factor.
 Objective function: max(1/|W|)
 or min(|W|) = min(1/2 |W|^2)
 
 To minimize error, for y = {1, -1}
 you constrain yi . f(xi) > 0, but
 even better: yi . f(xi) >= 1
 
 In total: minimize |W| under constraint
 yi . f(xi) >= 1
 
 By requiring >= 1, you create a "cushion"
 around the separator that you are certain
 has no data points in it. Must be done
 after normalization.
 
 
 */

#endif /* svm_h */
