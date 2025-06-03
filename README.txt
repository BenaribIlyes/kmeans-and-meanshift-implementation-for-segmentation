# tp_image

## 1. Description
The `tpTI` program is a tool for **image segmentation** using two methods:

1. **Custom K-Means (`kmeans_perso`)**
2. **Mean-Shift (`meanshift`)**

Features:
- Display and save segmented images
- Binarize segmented images for comparison
- Invert segmented images if needed
- Evaluate segmentation by comparing with a reference (ground truth) image

---

## 2. Algorithm Overview

### Custom K-Means (`kmeans_perso`)
K-Means is a clustering method that divides an image into `K` regions by minimizing intra-cluster variance.

**Steps:**
1. **Initialization**: Randomly choose `K` centroids.  
2. **Assignment**: Assign each pixel to the nearest centroid (Euclidean distance).  
3. **Update**: Recompute centroids as the mean of assigned pixels.  
4. **Convergence**: Stop when centroids change very little or after a fixed number of iterations.

**Characteristics:**
- Fast but requires specifying `K` in advance  
- Sensitive to initial centroid placement  
- Ideal for **quick and simple segmentation**

### Mean-Shift (`meanshift`)
Mean-Shift is a clustering method based on local pixel similarity.

**Steps:**
1. **Define a search window** (spatial bandwidth `hs`) around each pixel.  
2. **Compute the mean** of neighboring pixels in color space (color bandwidth `hc`).  
3. **Shift** the pixel toward this mean.  
4. **Repeat** until convergence.

**Characteristics:**
- No need to specify `K` in advance  
- Well-suited for **complex image segmentation**  
- Slower than K-Means but more accurate for textured or shaded objects

---

## 3. Usage

### Build Directory
All commands assume you are in the `tpTI/src/tp/build` directory after building with CMake (see Section 7).

### K-Means
```bash
./kmeans <image_file> <k> [ground_truth] <output_kmeans>
