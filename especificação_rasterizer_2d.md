__COMPUTER GRAPHICS I (DIM0451)__

__1st PROGRAMMING PROJECT: A SIMPLE 2D RASTERIZER__

# 1. Introduction

In this programming project assignment you should implement a simple 2D rasterizer. A [rasterizer](https://en.wikipedia.org/wiki/Rasterisation) is a program that takes in the description of vector shapes and converts that description into a raster image, i.e. a matrix of pixels.

Therefore, your rasterizer should receive as input a scene description file (coded in xml, json, yaml, or any markup languages of your liking) and outputs an image (PPM or PNG) that contains the rasterizaton of all Two-Dimensional (2D) primitives described in the input file.

The assignment goal is to introduce some of the classic 2D algorithms, designed to draw basic primitives such as lines, circle, polylines, etc., as well as algorithms to fill in those primitives with solid colors.


# 2. The 2D Raster Algorithms (_the project must support_)

## Drawing of 2D primitives

The requested 2D primitives are:

1. **line segment**, defined by two points. Choose one of the following algorithms:
    * [Digital Differential Analyzer](https://www.tutorialspoint.com/computer_graphics/line_generation_algorithm.htm) (DDA) algorithm
    * [Bresenham](https://www.geeksforgeeks.org/bresenhams-line-generation-algorithm/) algorithm
    * [Midpoint](https://www.geeksforgeeks.org/mid-point-line-generation-algorithm/) algorithm 
    * [Xiaolin's](https://dl.acm.org/citation.cfm?id=122734) anti-aliasing line drawing algorithm (extra credit)
2. **circles**, defined by a center point and a radius.
    * Midpoint algorithm
3. **circle arcs** (closed segment of a circle), which might be defined by a center point, a second point on the circle and an angle, for instance. 
4. **polylines**, i.e. a set of points that must be connected by lines.
5. **polygon**, i.e. a polyline in which the last point is connected to the first point of the set.
6. **Ellipses** or other **conics** (extra credit).

All these primitives might be draw with a **single pixel thickness** or multiple pixel thickness (extra credit).

## Color Filling algorithms

The solid color filling algorithms are:
1. [scan-line algorithm](https://en.wikipedia.org/wiki/Scanline_rendering)
2. [flood](https://en.wikipedia.org/wiki/Flood_fill) and/or [boundary fill](https://www.tutorialspoint.com/computer_graphics/polygon_filling_algorithm.htm_)

The solid color filling only works on closed primitives, i.e. polygons and conics.
Also, for the flood/boundary fill to work, the scene file must indicate the interior points that work as starting point for these algorithms.

## Anti-aliasing

You rasterizer also should provide at least one [**anti-aliasing algorithm**](https://en.wikipedia.org/wiki/Xiaolin_Wu%27s_line_algorithm). You may find a simple algorithm [here](https://en.wikipedia.org/wiki/Spatial_anti-aliasing#Simplest_approach_to_anti-aliasing).

## Extra Features

You may want to improve you rasterizer by providing mechanisms to:
1. create **layers**, so that primitives might be organized according to their `Z` coordinates.
2. create **groups**, so that you may group primitives together and assign a single name to them. For instance, image that you want to create a circle sector and filled that in with a solid color: this might be accomplished by creating a group that combines two line segments and a arc.
3. create **color palette**. You might define at some point in your scene file a set of colors (palette) assigned to names, so that these names may be later on referenced to while specifying a solid color to fill in a closed primitive.
4. create **viewports** or **viewboxes**. These entities define which parts of the world must be mapped to the output image. This is useful if you want to define a drawing coordinate system (world) different from the traditional image coordinate system (zero at the top left corner, positive `X` from left to right and positive `Y` from top to bottom).

# 3. Tasks

Your tasks in this assignment are the following:

1. Design a scene description language to support all the raster algorithms described above. Provide a manual document describing how to create a description file. _Have a look at how the [SVG](https://en.wikipedia.org/wiki/Scalable_Vector_Graphics) file format is specified to get some inspiration for your own scene description strategy._
2. Design a 2D rasterizer that reads in a scene description file and outputs the corresponding rasterized image as a [PPM](https://en.wikipedia.org/wiki/Netpbm_format) (P3 or P6) or [PNG](https://en.wikipedia.org/wiki/Portable_Network_Graphics) file.
3. Choose at least on extra feature to implement in your rasterizer.

