#ifndef TWEN_ADSR_H
#define TWEN_ADSR_H

#include "Utils.h"

class ADSR {
public:
	enum State {
		Idle = 0,
		Attack,
		Decay,
		Sustain,
		Release
	};

	ADSR()
		: m_attack(0.0f), m_decay(0.0f), m_sustain(1.0f), m_release(0.0f),
		m_targetRatioA(0.3f), m_targetRatioDR(0.0001f), m_out(0.0f)
	{}

	ADSR(float a, float d, float s, float r)
		: m_targetRatioA(0.3f), m_targetRatioDR(0.0001f), m_out(0.0f)
	{
		attack(a);
		decay(d);
		sustain(s);
		release(r);
	}

	float attack() const { return m_attack; }
	void attack(float v) {
		m_attack = v;
		m_attackCoef = coef(v, m_targetRatioA);
		m_attackBase = (1.0f + m_targetRatioA) * (1.0f - m_attackCoef);
	}

	float decay() const { return m_decay; }
	void decay(float v) {
		m_decay = v;
		m_decayCoef = coef(v, m_targetRatioDR);
		m_decayBase = (m_sustain - m_targetRatioDR) * (1.0f - m_decayCoef);
	}

	float release() const { return m_release; }
	void release(float v) {
		m_release = v;
		m_releaseCoef = coef(v, m_targetRatioDR);
		m_releaseBase = -m_targetRatioDR * (1.0f - m_releaseCoef);
	}

	float sustain() const { return m_sustain; }
	void sustain(float v) {
		m_sustain = v;
		m_decayBase = (m_sustain - m_targetRatioDR) * (1.0f - m_decayCoef);
	}

	float targetRatioA() const { return m_targetRatioA; }
	void targetRatioA(float v) {
		if (v < 0.000000001f)
			v = 0.000000001f;  // -180 dB
		m_targetRatioA = v;
		m_attackCoef = coef(m_attack, m_targetRatioA);
		m_attackBase = (1.0f + m_targetRatioA) * (1.0f - m_attackCoef);
	}

	float targetRatioDR() const { return m_targetRatioDR; }
	void targetRatioDR(float v) {
		if (v < 0.000000001f)
			v = 0.000000001f;  // -180 dB
		m_targetRatioDR = v;
		m_decayCoef = coef(m_decay, m_targetRatioDR);
		m_releaseCoef = coef(m_release, m_targetRatioDR);
		m_decayBase = (m_sustain - m_targetRatioDR) * (1.0f - m_decayCoef);
		m_releaseBase = -m_targetRatioDR * (1.0f - m_releaseCoef);
	}

	void gate(bool g);
	float sample();

	void reset();

private:
	State m_state;
	float m_attack,
		m_decay,
		m_release,
		m_sustain,
		m_attackBase,
		m_decayBase,
		m_releaseBase,
		m_attackCoef,
		m_decayCoef,
		m_releaseCoef,
		m_targetRatioA,
		m_targetRatioDR,
		m_out;

	float coef(float rate, float targetRatio);

};

#endif // TWEN_ADSR_H