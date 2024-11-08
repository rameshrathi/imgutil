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

## Folder Structure
- **Input Folder**: The folder containing the images to process.
- **Faces Folder**: The folder where cropped and sharpened face images will be stored.

Example folder structure:
/images image1.jpg image2.jpg /faces John_Doe_1.jpg Jane_Smith_1.jpg


## Command-Line Interface (CLI)

### Commands

1. **Run Face Detection Tool**
    - Command: `face_detect_tool --input <input_folder> --output <faces_folder>`
    - Description: Starts the tool, reads all images from the specified input folder, processes them, and saves the cropped faces in the output folder.

   **Parameters**:
    - `--input <input_folder>`: Path to the folder containing the images to process.
    - `--output <faces_folder>`: Path to the folder where the cropped face images will be saved.

   **Example**:
   ```bash
   face_detect_tool --input /path/to/images --output /path/to/faces


API
process_images(input_folder, output_folder, method="haar", sharpness_level=5, max_faces=0)
Processes images in the given input_folder, detects faces, sharpens them, and saves them to the output_folder.

Parameters:

input_folder (str): Path to the folder containing the input images.
output_folder (str): Path to the folder where the face images will be saved.
method (str, default="haar"): Method used for face detection ("haar", "dnn", "mtcnn").
sharpness_level (int, default=5): Level of sharpness to apply (0-10).
max_faces (int, default=0): Maximum number of faces to detect per image. If 0, all detected faces are processed.
Returns:

None.
Example:

python
Copy code
process_images("/path/to/images", "/path/to/faces", method="dnn", sharpness_level=7, max_faces=3)
detect_faces(image_path, method="haar")
Detects faces in a given image.

Parameters:

image_path (str): Path to the image file.
method (str, default="haar"): Method used for face detection ("haar", "dnn", "mtcnn").
Returns:

List of face bounding boxes (tuples) in the form (x, y, w, h) where:
x, y: Coordinates of the top-left corner of the face.
w, h: Width and height of the face.
Example:

python
Copy code
faces = detect_faces("/path/to/image.jpg", method="dnn")
crop_face(image_path, face_box)
Crops a face from an image.

Parameters:

image_path (str): Path to the image file.
face_box (tuple): Face bounding box in the form (x, y, w, h).
Returns:

Cropped face image.
Example:

python
Copy code
cropped_face = crop_face("/path/to/image.jpg", (50, 50, 100, 100))
sharpen_image(image)
Applies sharpening filter to an image.

Parameters:

image (numpy.ndarray): Image to be sharpened.
Returns:

Sharpened image.
Example:

python
Copy code
sharpened_face = sharpen_image(cropped_face)
save_face(face_image, person_name, output_folder)
Saves a cropped and sharpened face image to the output folder.

Parameters:

face_image (numpy.ndarray): The face image to be saved.
person_name (str): The name of the person detected in the face image.
output_folder (str): Path to the folder where the face image will be saved.
Returns:

None.
Example:

python
Copy code
save_face(sharpened_face, "John_Doe", "/path/to/faces")
Error Handling
File Not Found: If an image file does not exist or cannot be read, the tool will log an error and skip the file.
Invalid Method: If an unsupported face detection method is specified, the tool will use the default "haar" method and log a warning.
Permission Denied: If the tool cannot write to the specified output folder, it will log an error and terminate.
Logging
The tool logs details of its execution to a log file (face_detect_tool.log), which includes:

Image processing start and end times.
Any errors encountered during face detection, cropping, or saving.
Warnings for invalid configurations or unsupported methods.