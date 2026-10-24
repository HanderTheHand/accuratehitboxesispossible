// Random Jumpscare — único efeito: um jumpscare (imagem + áudio) em horário
// aleatório durante a gameplay. Nenhuma outra função do jogo é alterada.
#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <random>

using namespace geode::prelude;

namespace {
	// Tempo (em segundos de gameplay ativa) até o próximo jumpscare.
	constexpr float kFirstMin = 15.f;
	constexpr float kFirstMax = 75.f;
	constexpr float kNextMin  = 30.f;
	constexpr float kNextMax  = 120.f;

	// Cronologia do vídeo original (3,75 s): flash rápido, segura, fade longo.
	constexpr float kFadeIn   = 0.15f;
	constexpr float kHold     = 0.85f;
	constexpr float kFadeOut  = 2.75f;
	constexpr float kTotal    = kFadeIn + kHold + kFadeOut;

	float randomRange(float lo, float hi) {
		static std::mt19937 rng{std::random_device{}()};
		return std::uniform_real_distribution<float>(lo, hi)(rng);
	}

	void showJumpscare() {
		auto director = CCDirector::sharedDirector();
		auto scene = director->getRunningScene();
		if (!scene) return;

		auto id = "jumpscare-overlay"_spr;
		if (scene->getChildByID(id)) return; // já está na tela

		auto sprite = CCSprite::create("jumpscare.png"_spr);
		if (!sprite) {
			log::warn("jumpscare.png não encontrado nos recursos do mod");
			return;
		}

		auto win = director->getWinSize();
		auto size = sprite->getContentSize();
		float scale = std::max(win.width / size.width, win.height / size.height);

		sprite->setID(id);
		sprite->setScale(scale);
		sprite->setPosition({win.width / 2.f, win.height / 2.f});
		sprite->setOpacity(0);
		scene->addChild(sprite, 100000);

		// Visual: flash -> segura -> fade (igual ao vídeo) e remove no fim.
		sprite->runAction(CCSequence::create(
			CCFadeIn::create(kFadeIn),
			CCDelayTime::create(kHold),
			CCFadeOut::create(kFadeOut),
			CCCallFunc::create(sprite, callfunc_selector(CCNode::removeFromParent)),
			nullptr
		));
		// Leve zoom durante todo o efeito.
		sprite->runAction(CCScaleTo::create(kTotal, scale * 1.10f));

		// Áudio original, disparado no mesmo frame do visual.
		FMODAudioEngine::sharedEngine()->playEffect("jumpscare.ogg"_spr, 1.f, 0.f, 1.f);
	}
}

class $modify(JumpscarePlayLayer, PlayLayer) {
	struct Fields {
		float elapsed = 0.f;
		float nextAt = -1.f;
		bool first = true;
	};

	void postUpdate(float dt) {
		PlayLayer::postUpdate(dt);

		// Só conta tempo enquanto o jogador está vivo e jogando.
		if (!m_player1 || m_player1->m_isDead) return;

		auto f = m_fields.self();
		if (f->nextAt < 0.f) {
			f->nextAt = randomRange(kFirstMin, kFirstMax);
		}

		f->elapsed += dt;
		if (f->elapsed >= f->nextAt) {
			showJumpscare();
			f->elapsed = 0.f;
			f->nextAt = randomRange(kNextMin, kNextMax);
		}
	}
};
