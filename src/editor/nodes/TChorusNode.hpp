#ifndef T_CHORUS_NODE_H
#define T_CHORUS_NODE_H

#include "TNode.h"
#include "../../TGen.h"

#include "imgui.h"

class TChorusNode : public TNode {
public:
	TChorusNode(float sampleRate, float dt=50, float cr=1.0f, float cd=1.0f)
		: TNode("Chorus", 140, 140),
			sampleRate(sampleRate), delayTime(dt), chorusRate(cr), chorusDepth(cd)
	{
		m_lfo = new TOsc(sampleRate);
		m_lfo->amplitude(1.0f);
		m_lfo->waveForm(TOsc::Sine);

		m_wv = new TWaveGuide();
		m_wv->clear();

		addInput("In");
		addOutput("Out");
	}

	virtual ~TChorusNode() {
		delete m_lfo;
		m_lfo = nullptr;
		delete m_wv;
		m_wv = nullptr;
	}

	void gui() {
		ImGui::DragFloat("Rate", &chorusRate, 0.01f, 0.001f, 20000.0f);
		ImGui::DragFloat("Depth", &chorusDepth, 0.1f, 0.01f, 10.0f);
		ImGui::DragFloat("Delay", &delayTime, 0.1f, 1, 100);
	}

	void solve() {
		float sgn = m_lfo->sample(chorusRate) * chorusDepth;
		float sgnDT = sgn * delayTime;
		dt = sgnDT + delayTime;
		float out = m_wv->sample(getInput(0), 0.0f, dt);
		setOutput(0, (out + getInput(0)) * 0.5f);
	}

	virtual void save(JSON& json) {
		TNode::save(json);
		json["type"] = type();
		json["sampleRate"] = sampleRate;
		json["chorusRate"] = chorusRate;
		json["delayTime"] = delayTime;
		json["chorusDepth"] = chorusDepth;
	}

	float sampleRate;
	float lpOut;
	double dt;

	float chorusRate, chorusDepth, delayTime;

	static std::string type() { return "Chorus"; }
private:
	TOsc* m_lfo;
	TWaveGuide* m_wv;
};

#endif // T_CHORUS_NODE_H