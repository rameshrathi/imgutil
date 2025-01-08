# Image Face Detection Tool

## Overview
The **Image Face Detection Tool** is a utility designed to read images from a specified folder, detect faces in the images, crop the detected face regions, sharpen them for better clarity, and save them in a designated "faces" folder. The saved face images are named based on the person's name detected in the image.

## Features
- **Read Images from Folder**: Supports bulk image processing by reading all images from a user-defined folder.
- **Face Detection**: Detects faces in each image using facial recognition algorithms.
- **Face Cropping**: Crops the detected face(s) from each image.
- **Face Sharpening**: Applies sharpening filters to the cropped face images.
- **Saving Cropped Faces**: Saves the sharpened face images in a "faces" folder, with filenames based on the detected person’s name.
- **Multiple Faces**: Supports handling multiple faces in a single image.

### To generate xcode project
`cmake -G "Xcode" -B xcode`