# Masterthesis Collision System: AI vs. Classical Algorithms

This repository contains the practical part of Juri Wiechmann's master's thesis, which focuses on the development and evaluation of collision detection systems in game engines. The goal is to compare classical algorithms with an AI-driven approach using a Feedforward Neural Network (FNN).

## Project Description

The master's thesis investigates whether deep learning algorithms can be effectively applied to partition space for optimizing collision detection in 2D simulations. Traditional algorithms like grid partitioning and quadtree are compared with a trained FNN.

**Key Components of the Project:**

- **Implementation of Classical Algorithms:** Grid and Quadtree partitioning
- **Implementation of an AI-Driven Approach:** Feedforward Neural Network (FNN) using deep learning
- **Comparison Framework:** Runtime, collision precision, and number of collision checks

### Limitations

- The training was not successful because the evaluation function is not differentiable, preventing the use of backpropagation.
- Evolutionary training with multiple agents and random adjustments to the neural network would have been a more effective approach.
