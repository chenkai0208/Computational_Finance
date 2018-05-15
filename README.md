# Computational_Finance
Computational Finance
Students learn how to implement financial option-pricing and risk-management models using C++, building on previous and concurrent courses on object-oriented programming with C++, numerical analysis, and mathematical finance. MATLAB, Python, and Excel-VBA may also be used, though primarily as tools for benchmarking and C++ code interfacing. Numerical methods discussed include Monte Carlo simulation, finite difference, finite element, and spectral element solution of partial differential equations, binomial and trinomial trees, the fast Fourier transform (FFT). Asset classes discussed include equities, fixed income and interest rates, foreign exchange, and commodities, though the majority of applications will be for equity derivatives for simplicity and access to market data.

## Project 1: European-style Vanila Option
In this project, we use some classical numerical methods to conduct Monte Carlo simulations for the purpose of pricing European vanila options. The detailed explanation on the methods and procedures can be found in [report1](Kai_Chen_project1/Kai_Chen_report1.pdf).

## Project 2: Barrier Option Pricing
In this project, Monte Carlo simulation is used to price barrier options. Antithetic variates method is implemented to reduce the variance of results. In addition, the control variates algorithm is conducted to raise the accuracy of pricing. Check more detail in [report2](Kai_Chen_project2/Kai_Chen_report2.pdf).

## Project 3: Random Number Generator
In this project, some classical random number generators are implemented in a C++ configuration. The generated random numbers are first used for option pricing. Then they are used to generate another sets of normal random numbers, which are compared with the true probability distribution for a standard normal variable. All the details are in [report3](Kai_Chen_project3/Kai_Chen_report3.pdf).
