chopiner
========

chopiner is a program to break up n-plets - in sheet music - into distinct sub-beats. In Romantic-era classical music, this compositional technique became more prominently used, as music became more expressive. As a result, learning a Romantic-era piece can be rhythmically daunting -- for example, having one hand play seven even notes, while another hand plays four even notes.

Here's an example (In 3-4 time): let the right hand's notes be three notes in the time of two quarter-notes, where the last of the three has half the duration of the other two, followed by a quarter-note (in the program's syntax, represented as "(ooo_)<2>o"), and let the left hand have three even quarter-notes. The resulting n-plet decomposition is as follows:

	RH: o---o---o-o----
	LH: o----o----o----
