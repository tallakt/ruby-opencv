require 'opencv'
include OpenCV

m = CvMat.new 3, 3, CV_32F
p CvMat::estimate_affine_3d m, m
