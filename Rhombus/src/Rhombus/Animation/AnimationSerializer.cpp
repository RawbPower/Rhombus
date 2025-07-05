#include "rbpch.h"
#include "AnimationSerializer.h"

#include "Rhombus/ECS/Components/AnimatorComponent.h"
#include "Rhombus/ECS/Components/SpriteRendererComponent.h"

#include <fstream>

#define YAML_CPP_STATIC_DEFINE		// Needed for yaml static library to work for some reason
#include <yaml-cpp/yaml.h>

namespace rhombus
{
	void AnimationSerializer::SerializeAnimations(const std::string& filepath, Entity entity)
	{
		if (!entity.HasComponent<AnimatorComponent>())
		{
			return;
		}

		AnimatorComponent& animator = entity.GetComponent<AnimatorComponent>();

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Animator" << YAML::Value << entity.GetName();
		out << YAML::Key << "Clips" << YAML::Value << YAML::BeginSeq;
		for (int clipIndex = 0; clipIndex < animator.GetAnimationCount(); clipIndex++)
		{
			AnimationClip& clip = animator.GetAnimationClipRef(clipIndex);
			out << YAML::BeginMap; // Entity
			out << YAML::Key << "Name" << YAML::Value << clip.m_name;
			out << YAML::Key << "Looping" << YAML::Value << clip.m_looping;
			out << YAML::Key << "Samples" << YAML::Value << YAML::BeginSeq;
			for (int i = 0; i < clip.m_samples.size(); i++)
			{
				out << YAML::BeginMap;

				out << YAML::Key << "Duration" << YAML::Value << clip.m_samples[i].m_duration;
				out << YAML::Key << "Sprite" << YAML::Value << entity.GetName();
				out << YAML::Key << "Frame" << YAML::Value << clip.m_samples[i].m_spriteFrame;
				out << YAML::EndMap;
			}
			out << YAML::EndSeq;
			out << YAML::EndMap;
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	bool AnimationSerializer::DeserializeAnimations(const std::string& filepath, Entity entity)
	{
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(filepath);
		}
		catch (YAML::ParserException e)
		{
			Log::Error("Failed to load animations file '%s'\n     %s", filepath, e.what());
			return nullptr;
		}

		
		if (!entity.HasComponent<AnimatorComponent>())
		{
			return false;
		}

		AnimatorComponent& animator = entity.GetComponent<AnimatorComponent>();
		
		auto animClips = data["Clips"];
		if (animClips)
		{
			for (auto clipData : animClips)
			{
				AnimationClip clip;
				clip.m_name = clipData["Name"].as<std::string>();
				clip.m_looping = clipData["Looping"].as<bool>();
				clip.m_duration = 0.0f;
				auto samplesData = clipData["Samples"];
				if (samplesData)
				{
					for (auto sampleData : samplesData)
					{
						AnimationSample sample;
						sample.m_duration = sampleData["Duration"].as<float>();
						clip.m_duration += sample.m_duration;
						sample.m_spriteFrame = sampleData["Frame"].as<int>();
						clip.m_samples.push_back(sample);
					}
				}
				animator.AddAnimation(clip);
			}
		}

		return true;
	}
}