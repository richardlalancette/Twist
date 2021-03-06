#ifndef TWEN_SAMPLE_NODE_H
#define TWEN_SAMPLE_NODE_H

#include "../Node.h"
#include "../NodeGraph.h"
#include "../intern/Sample.h"

class SampleNode : public Node {
	TWEN_NODE(SampleNode, "Sample")
public:
	SampleNode(int sampleID=0) : Node() {
		addInput("Gate");
		addInput("Amp");
		addOutput("Out");

		addParam("Sample", {}, sampleID);
		addParam("Amp", 0.0f, 1.0f, 1.0f, 0.05f, NodeParam::KnobRange, true);
	}

	void setup() {
		u32 id = paramOption(0);
		RawSample* sle = parent()->getSample(parent()->getSampleID(parent()->getSampleNames()[id]));
		if (sle != nullptr) {
			sample = Sample(sle->data, sle->sampleRate, sle->duration);
		}
	}

	void solve() {
		if (sample.valid()) {
			bool gate = in(0) > 0.5f ? true : false;
			sample.gate(gate);
			out(0) = sample.sample() * in(1, 1);
		}
	}

	Sample sample;
};

#endif // TWEN_SAMPLE_NODE_H
