cmake_minimum_required(VERSION 3.10)
project(MyProjectName LANGUAGES CXX)

# Spécifier la norme C++ (ici C++11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Recherche d'OpenCV
find_package(OpenCV REQUIRED)

# Ajouter les répertoires d'inclusion
include_directories(${OpenCV_INCLUDE_DIRS})

# Définir l'exécutable kmeans
add_executable(kmeans kmeans.cpp ocv_utils.cpp kmeans_functions.cpp)
target_link_libraries(kmeans ${OpenCV_LIBS})

# Définir l'exécutable meanshift
add_executable(meanshift meanshift.cpp ocv_utils.cpp meanshift_functions.cpp kmeans_functions.cpp)
target_link_libraries(meanshift ${OpenCV_LIBS})

# Définir l'exécutable tp_image
add_executable(tp_image tp_image.cpp ocv_utils.cpp kmeans_functions.cpp meanshift_functions.cpp)
target_link_libraries(tp_image ${OpenCV_LIBS})