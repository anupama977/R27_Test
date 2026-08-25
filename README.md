# R27_test

<p align="center">
  <img src="https://github.com/teamrudra/r26_test/blob/main/misc/rover.webp" width="480" height="480"/>


Problem Statement
--
This test evaluates your ability to understand, debug, and implement functionality in an existing C-based embedded/robotics codebase.

The test has three parts:

COBS Encoding & Decoding
Multithreading & Synchronization
Differential-Drive Rover

You are given an existing codebase with partially implemented functionality. Your task is to understand the code, identify the issues, implement the required changes, and verify your solution.

Part 1 — COBS Encoding & Decoding
--
Description

The first part focuses on COBS (Consistent Overhead Byte Stuffing), commonly used in embedded communication.

Relevant files:

src/en_dc.c
lib/en_dc.h


What you need to do

Understand the existing encoder and decoder.
Fix any incorrect or incomplete logic.
Handle zero bytes and buffer limits correctly.
Handle invalid/edge-case inputs.
Verify that encoding and then decoding returns the original data.

Part 2 — Multithreading & Synchronization
--
Description

The second part focuses on POSIX threads, mutexes, semaphores, and message queues.

Relevant files:

src/comms.c
src/mutex.c
src/main.c


The application uses a circular message queue and multiple threads to pass data between different parts of the system.

What you need to do

Review and correct the existing implementation.

Pay attention to:

Producer/consumer synchronization.
Queue full and empty conditions.
Mutex and semaphore usage.
Reader/writer synchronization.
Race conditions.
Deadlocks.
Correct initialization and cleanup of resources.

Keep the existing architecture where possible.

Part 3 — Differential-Drive Rover
-
Description

The final part involves controlling a differential-drive rover.

The rover has:

X/Y position
Heading
Left wheel velocity
Right wheel velocity

Relevant files:

src/drive.c
lib/drive.h


A rover simulator is already provided.

What you need to do

Complete/fix the drive_to_target() functionality so that the rover reaches the target.

Your implementation should:
--
Calculate the direction to the target.
Account for the rover's heading.
Generate appropriate left/right wheel velocities.
Respect velocity limits.
Handle different target directions.
Handle heading wraparound.
Stop when the target is reached.
Avoid infinite loops.

Path planning and PID control are not required.

Getting Started

First understand the project structure and read the relevant source files before making changes.

Build the project:
--
make build


Run the checks:

make check


To clean the build:

make clean


Time Limit

90 minutes

Try to attempt all three parts. If you get stuck on one section, move on and return to it later.

Use of Online Resources & AI

You may use Google, documentation, technical resources, and AI tools during the test.

However, document what you used and how it helped in your final README.

We are interested in your understanding and problem-solving process, not just the final code.

Your README

After completing the test, add a short explanation covering:
--
What you understood about each part.
Your approach to solving the problems.
What you changed and why.
How you tested your implementation.
Any challenges you faced.
Any external resources or AI tools you used.

Important
--
Read the existing code before modifying it.
Do not unnecessarily rewrite working components.
Do not modify tests just to make them pass.
Handle edge cases and invalid inputs.
Keep your code clean and readable.
Make sure the project builds and make check runs successfully.

If you cannot complete a section, document what you attempted and where you got stuck.

Submission

Before submitting:

 Attempt all three sections.
 Run make build.
 Run make check.
 Update the README with your approach.
 Document external/AI resources used.
 Commit and push your changes to your fork.
 Submit your repository link.

Evaluation
--
The submission will be evaluated on:

Correctness
C programming skills
Debugging and problem-solving
Understanding of the existing code
Concurrency and synchronization
Algorithmic reasoning
Edge-case handling
Code quality
Ability to explain your solution

Good luck!
# Google Form
https://forms.gle/A8CaByv4ohfrCmmWA
GOOD LUCK!

<p align="center">
  <img src="https://github.com/teamrudra/r25-test/blob/main/datasheets/feynman-simple.jpg" width="600" height="600"/>
</p>
     

