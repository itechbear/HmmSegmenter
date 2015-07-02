# HmmSegmenter
A Hidden-Markov-Model based Chinese word segmenter

Introduction
============

This is a Chinese word segmenter for demostrating purposes.

Compile
=======

 - $ mkdir build
 - $ cd build
 - $ cmake ..
 - $ make
 
Usage
=====

 - ./HmmSegmenter --training_file=data/msr_training.utf8 --text="在一系列利好政策消息下，7月1日，沪指下跌223.52点，两市800只个股跌停。"
