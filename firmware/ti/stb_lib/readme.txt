;*************************************************************************************
;************************** Software Test Bench (STB) Support Library ****************
;*************************************************************************************
Thank you for trying C28x Software Collateral. 
The software is installed in C:\TIDCS\C28\STB_LIB directory.

Texas Instruments has developed a powerful, yet easy to use software collateral for 
general market to use on all of our TMS320x28x family of products. These modules are 
typically used in computationally intensive real-time applications where optimal 
execution speed is critical. By using these routines, you can achieve execution speeds 
considerably faster than equivalent code written in standard ANSI C language. 
In addition to providing ready-to-use DSP functions, this can significantly shorten 
your application development time.

To facilitate evaluation and deployment of these modules, they are made available as 
Software Test Benches (STBs) which run as code composer projects on readily 
available EVMs or eZdsp hardware platforms. Each STB focuses on a particular 
software module and shows the customer how to invoke it, pass variable or data to it, 
and how to link it into their systems. Where possible, the module under evaluation is 
made to interact with other modules such as signal generators, which can provide input 
stimulus and data-logging modules to examine a module's response in a real-time environment. 
This helps customers to get a more realistic feel of the software module's capability and applicability. 


This library contains the support modules for Software Test Bench (STB) framework.

Version History:
================

Version 0.9a dated 12/06/2003 

1. Some the math function had .ref being used in the *.asm, it is changed to .def 
   Because of this bug, compiler was reporting that the function name is not available.

2. Most of the functions did not clear the OVM flag at the end, it is modified.

3. ASP & NASP is been used within functions, we should not disturb the ASP bit.
   It is meant for only interrupt handler. Called function should not change this bit.
