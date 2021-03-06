#ifndef TWEN_DELAY_LINE_NODE_H
#define TWEN_DELAY_LINE_NODE_H

#include "../Node.h"
#include "../intern/WaveGuide.h"

class DelayLineNode : public Node {
	TWEN_NODE(DelayLineNode, "Delay Line")
public:
	DelayLineNode(float sampleRate=44100.0f, float fb=0, float dl=0)
		: Node(), m_wv(WaveGuide(sampleRate))
	{
		addInput("In");
		addOutput("Out");

		addParam("Feedback", 0.0f, 1.0f, fb, 0.05f, NodeParam::Range, false, 90);
		addParam("Delay", 0.0f, 10.0f, dl, 0.05f, NodeParam::Range, false, 90);
	}

	void solve() {
		out(0) = m_wv.sample(in(0), param(0), param(1));
	}

private:
	WaveGuide m_wv;

};

#endif // TWEN_DELAY_LINE_NODE_H
