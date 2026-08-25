# R27_test

<p align="center">
  <img src="https://github.com/teamrudra/r26_test/blob/main/misc/rover.webp" width="480" height="480"/>

R27 Embedded C Coding Test
Problem Statement

This test is designed to evaluate your ability to understand, debug, and implement functionality in an existing C-based embedded/robotics codebase.

You are given an existing project containing multiple components that work together as part of a simplified robotics communication and control system.

The test consists of three main coding sections:

COBS Encoding and Decoding
Multithreading, Message Queue and Synchronization
Differential-Drive Rover Control

Your task is to understand the existing implementation, identify the issues or incomplete functionality, and implement the required changes.

You are not expected to build the project from scratch.

The main objective is to demonstrate your ability to:

Read and understand an unfamiliar C codebase.
Debug existing code.
Work with memory, pointers and buffers.
Understand communication encoding/decoding.
Work with POSIX threads and synchronization.
Implement a differential-drive control algorithm.
Handle edge cases and invalid inputs.
Test and validate your implementation.
Clearly explain your technical decisions.
Test Structure

The test contains three major sections.

Part 1 — COBS Encoding and Decoding
Description

The first section focuses on COBS (Consistent Overhead Byte Stuffing).

COBS is commonly used in embedded communication systems to encode data so that a reserved delimiter byte does not appear inside the encoded payload.

The project already contains an implementation of the COBS encoder and decoder.

Relevant files include:

src/en_dc.c
lib/en_dc.h


You should first understand how the existing encoder and decoder work and how they are used by the rest of the application.

What You Have To Do

Review the existing COBS implementation and make the required corrections.

You should verify:

How input data is encoded.
How encoded data is decoded.
How the encoder handles zero bytes.
How the decoder reconstructs the original data.
Buffer-size handling.
Pointer/input validation.
Error/status handling.
Empty input.
Boundary conditions.
Whether encoding followed by decoding produces the original input.

The implementation should behave correctly for the test cases provided by the project.

Expected Outcome

For valid input:

Original Data
      ↓
COBS Encode
      ↓
Encoded Data
      ↓
COBS Decode
      ↓
Original Data


The decoded result should match the original data.

Part 2 — Multithreading, Message Queue and Synchronization
Description

The second section evaluates your understanding of POSIX threads, mutexes, semaphores, producer/consumer systems, and shared data synchronization.

Relevant files include:

src/comms.c
src/mutex.c
src/main.c


The application contains multiple threads that communicate with one another.

A message queue is used to transfer data between producers and consumers.

The queue is implemented using a fixed-size circular buffer and synchronization primitives are used to control access to the shared data.

The project also contains reader/writer synchronization logic.

Message Queue

The queue provides functionality to:

Initialize the queue.
Add messages.
Remove messages.
Destroy the queue.

The queue must correctly handle situations where:

The queue is empty.
The queue is full.
Multiple threads access the queue.
A producer and consumer access the queue at the same time.
Reader/Writer Synchronization

The project also contains shared-data access using a reader/writer synchronization mechanism.

The expected behaviour is that:

Multiple readers may access shared data concurrently.
A writer requires exclusive access.
Readers should not access the data while a writer is modifying it.
Writers should not modify data while readers are accessing it.
What You Have To Do

Read the existing threading implementation and identify problems in the synchronization logic.

You should verify:

Mutex initialization and usage.
Semaphore initialization and usage.
Producer/consumer synchronization.
Circular queue handling.
Queue full/empty conditions.
Shared-data protection.
Reader/writer synchronization.
Race conditions.
Deadlock possibilities.
Resource cleanup.
Thread termination.

Make the required changes while keeping the existing project architecture as much as reasonably possible.

Expected Outcome

The application should be able to safely pass messages between threads without:

Losing messages unexpectedly.
Corrupting shared data.
Accessing data without appropriate synchronization.
Deadlocking.
Incorrectly handling full or empty queues.
Part 3 — Differential-Drive Rover
Description

The third section focuses on controlling a differential-drive rover.

The rover has two independently controlled wheels:

             Forward
                ↑
        ┌───────────────┐
        │               │
 Left   │     ROVER     │   Right
 Wheel  │               │   Wheel
        └───────────────┘


The rover has:

A current X/Y position.
A current heading.
A left wheel velocity.
A right wheel velocity.
A target X/Y position.

The objective is to move the rover from its current position towards the target.

The relevant implementation is located in:

src/drive.c
lib/drive.h


A rover simulator is already provided by the project.

What You Have To Do

Complete/fix the drive_to_target() functionality.

Your implementation should:

Validate the input parameters.
Determine the direction from the rover to the target.
Account for the rover's current heading.
Calculate appropriate wheel velocities.
Control the left and right wheels independently.
Respect the allowed wheel velocity limits.
Handle targets located in different directions.
Correctly handle heading wraparound.
Detect when the rover has reached the target.
Stop the rover once the target is reached.
Avoid infinite loops.
Handle the maximum iteration condition correctly.
Work with the provided rover simulator.
Important

Path planning and PID control are not required.

The task is to implement the required differential-drive behaviour using the existing rover model and simulator.

Do not replace the simulator or redesign the overall rover architecture.

How The Three Parts Connect

The three sections form a simplified communication-to-control pipeline.

The overall flow is:

Input Coordinates
       │
       ▼
 Communication
       │
       ▼
 COBS Encoding
       │
       ▼
 COBS Decoding
       │
       ▼
 Thread Synchronization
       │
       ▼
 Message Queue
       │
       ▼
 Target Coordinate
       │
       ▼
 Differential Drive
       │
       ▼
 Rover Movement


The purpose of the test is to evaluate both individual component-level understanding and your ability to understand how these components interact inside an existing application.

What You Have To Do
Step 1 — Understand The Project

Before making changes, inspect the repository and understand the existing architecture.

Pay particular attention to:

lib/
src/
input/
result/


Important source files include:

src/comms.c
src/en_dc.c
src/mutex.c
src/drive.c
src/main.c

lib/en_dc.h
lib/read.h
lib/drive.h


Do not immediately start changing code.

First understand:

What each module does.
How data moves between modules.
Which functions are called by other modules.
What the expected behaviour of each function is.
Step 2 — Build The Project

From the root directory of the repository, run:

make build


Make sure the project builds successfully before making major changes.

Step 3 — Run The Checks

Run:

make check


Use the provided checks to determine whether your implementation behaves correctly.

If you need to clean the build:

make clean


Then rebuild:

make build


And run:

make check

Recommended Approach

You should work through the test in the following order.

1. Understand

Read the relevant source and header files.

Do not assume that the function names completely describe what the code is doing.

2. Identify

Find the incomplete, incorrect, or unsafe parts of the implementation.

Think about:

Normal cases.
Edge cases.
Invalid inputs.
Memory safety.
Thread safety.
Numerical correctness.
3. Implement

Make the necessary changes.

Avoid unnecessary modifications to unrelated code.

4. Test

Build and run the provided checks.

Test individual pieces where possible.

5. Debug

If a test fails:

Understand why it failed.
Trace the data flow.
Inspect the relevant function.
Fix the underlying problem rather than hard-coding an expected result.
Time Limit

The total duration of the test is:

90 Minutes

Manage your time carefully.

You are expected to attempt all three sections.

If you are unable to complete a particular section, move on to the next one and return to it if time permits.

A partially completed solution with a clear explanation is preferable to spending the entire test on one problem.

Use Of Online Resources And AI

You are allowed to use external resources during the test.

You may use:

Google
C documentation
POSIX documentation
Technical articles
Datasheets
Stack Overflow
AI tools
Other relevant online resources

However, we are also interested in your problem-solving process and understanding.

If you use an external resource or AI, document it in your final README.

For example:

Resource:
POSIX pthread documentation

Used for:
Understanding mutex and thread synchronization.


Or:

AI:
Used AI to understand the behaviour of the COBS encoding algorithm.

How it helped:
It clarified how zero bytes are represented in the encoded stream.

What I implemented:
I then implemented and tested the logic myself.


Do not simply copy an answer without understanding it.

You may be asked to explain your implementation during the evaluation.

Candidate Documentation

After completing the coding tasks, update the README with a short explanation of your solution.

Your documentation should contain the following sections.

1. Understanding

Explain what you understood about:

The COBS encoder/decoder.
The communication flow.
The message queue.
Producer/consumer synchronization.
Reader/writer synchronization.
Differential-drive movement.
2. Approach

Explain how you approached the problems.

For example:

How did you identify the issue?
How did you break the problem down?
What assumptions did you make?
Why did you choose your particular approach?
3. Implementation

Explain the changes you made.

Include:

What files/functions you modified.
What was wrong or incomplete.
How you fixed it.
Any important implementation decisions.
4. Testing

Explain how you tested your solution.

Include:

Build results.
make check results.
Any additional tests you performed.
Edge cases you considered.
5. Challenges

Mention:

Where you got stuck.
What problems you encountered.
How you debugged them.
What you learned.
6. External Resources

List any:

Documentation
Websites
Articles
AI tools
Other resources

that you used during the test.

Important Instructions
Read the existing code before modifying it.
Do not unnecessarily rewrite working components.
Do not modify the test cases simply to make them pass.
Do not hard-code expected outputs.
Keep your changes focused on the required functionality.
Write readable and maintainable C code.
Consider invalid inputs and edge cases.
Pay attention to memory and resource management.
Pay attention to thread safety.
Test your implementation before submitting.

If you cannot complete a section, do not remove it or hide the incomplete work.

Instead, document:

What you understood.
What you attempted.
Where you got stuck.
How you would continue solving it.
Submission Requirements

Before submitting your work:

Complete as much of the three sections as possible.
Make sure the project builds.
Run the provided checks.
Update the README with your understanding and approach.
Document any external resources or AI assistance.
Commit your changes.
Push the changes to your fork.
Submit the URL of your forked repository through the provided submission form.
We also want to understand how you arrived at your solution.

Final Checklist

Before submitting, make sure you have:

 Attempted the COBS encoding/decoding section.
 Attempted the multithreading/message queue section.
 Attempted the differential-drive section.
 Built the project successfully.
 Run make check.
 Tested important edge cases.
 Updated the README.
 Documented your approach.
 Documented external resources/AI usage.
 Committed your changes.
 Pushed your changes to your fork.
 Submitted the google form at the end with your github repo link.
Good Luck!
Take your time to understand the existing code, think about the problem before changing it, and make sure you can explain the decisions behind your implementation.

# Google Form
https://forms.gle/A8CaByv4ohfrCmmWA

<p align="center">
  <img src="https://github.com/teamrudra/r25-test/blob/main/datasheets/feynman-simple.jpg" width="600" height="600"/>
</p>
     

