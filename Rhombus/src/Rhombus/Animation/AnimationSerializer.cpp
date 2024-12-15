#include "rbpch.h"
#include "AnimationSerializer.h"

#include "Rhombus/ECS/Components/AnimatorComponent.h"

#include <fstream>

#define YAML_CPP_STATIC_DEFINE		// Needed for yaml static library to work for some reason
#include <yaml-cpp/yaml.h>

namespace rhombus
{
	void AnimationSerializer::SerializeAnimations(const std::string& filepath, Entity entity)
	{
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