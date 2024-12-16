#include "ak_wwise_tree_data_source.h"

void AkWwiseTreeDataSource::init(AkWwiseTree* p_tree, WwiseObjectType p_picker_type)
{
	tree = p_tree;
	tree->clear();
	project_root = create_project_root_item();

	switch (p_picker_type)
	{
		case WwiseObjectType::AuxBus:
			project_root->add_wwise_item_child(build_object_type_tree(WwiseObjectType::AuxBus));
			break;
		case WwiseObjectType::Event:
			project_root->add_wwise_item_child(build_object_type_tree(WwiseObjectType::Event));
			break;
		case WwiseObjectType::Soundbank:
			project_root->add_wwise_item_child(build_object_type_tree(WwiseObjectType::Soundbank));
			break;
		case WwiseObjectType::State:
			project_root->add_wwise_item_child(build_object_type_tree(WwiseObjectType::State));
			break;
		case WwiseObjectType::Switch:
			project_root->add_wwise_item_child(build_object_type_tree(WwiseObjectType::Switch));
			break;
		case WwiseObjectType::GameParameter:
			project_root->add_wwise_item_child(build_object_type_tree(WwiseObjectType::GameParameter));
			break;
		case WwiseObjectType::Trigger:
			project_root->add_wwise_item_child(build_object_type_tree(WwiseObjectType::Trigger));
			break;
		case WwiseObjectType::AcousticTexture:
			project_root->add_wwise_item_child(build_object_type_tree(WwiseObjectType::AcousticTexture));
			break;
		default:
			UtilityFunctions::push_error(
					"WwiseGodot: Trying to create a AkWwiseTreeItem with non-existing WwiseObjectType.");
			break;
	}
}

void AkWwiseTreeDataSource::init_browser_tree(AkWwiseTree* p_tree)
{
	tree = p_tree;
	tree->clear();
	project_root = create_project_root_item();

	project_root->add_wwise_item_child(build_object_type_tree(WwiseObjectType::AuxBus));
	project_root->add_wwise_item_child(build_object_type_tree(WwiseObjectType::Event));
	project_root->add_wwise_item_child(build_object_type_tree(WwiseObjectType::Soundbank));
	project_root->add_wwise_item_child(build_object_type_tree(WwiseObjectType::State));
	project_root->add_wwise_item_child(build_object_type_tree(WwiseObjectType::Switch));
	project_root->add_wwise_item_child(build_object_type_tree(WwiseObjectType::GameParameter));
	project_root->add_wwise_item_child(build_object_type_tree(WwiseObjectType::Trigger));
	project_root->add_wwise_item_child(build_object_type_tree(WwiseObjectType::AcousticTexture));
}

AkWwiseTreeItem* AkWwiseTreeDataSource::create_project_root_item()
{
	AkWwiseTreeItem* root = memnew(AkWwiseTreeItem);
	root->init(tree, nullptr, "WwiseProject", String(), WwiseObjectType::Project);

	return root;
}

AkWwiseTreeItem* AkWwiseTreeDataSource::build_object_type_tree(WwiseObjectType p_object_type)
{
	AkWwiseTreeItem* root_element = memnew(AkWwiseTreeItem);

	switch (p_object_type)
	{
		case WwiseObjectType::None:
		{
			UtilityFunctions::push_error(
					"WwiseGodot: Trying to create a AkWwiseTreeItem with non-existing WwiseObjectType.");
			break;
		}
		case WwiseObjectType::AuxBus:
		{
			root_element =
					build_tree("Master-Mixer Hierarchy", WwiseProjectInfo::get_singleton()->get_data()->get_bus_root());
			break;
		}
		case WwiseObjectType::Event:
		{
			root_element = build_tree("Events", WwiseProjectInfo::get_singleton()->get_data()->get_event_root());
			break;
		}
		case WwiseObjectType::Soundbank:
		{
			root_element = build_tree("SoundBanks", WwiseProjectInfo::get_singleton()->get_data()->get_bank_root());
			break;
		}
		case WwiseObjectType::State:
		{
			root_element = build_tree("States", WwiseProjectInfo::get_singleton()->get_data()->get_state_root());
			break;
		}
		case WwiseObjectType::Switch:
		{
			root_element = build_tree("Switches", WwiseProjectInfo::get_singleton()->get_data()->get_switch_root());
			break;
		}
		case WwiseObjectType::GameParameter:
		{
			root_element = build_tree(
					"Game Parameters", WwiseProjectInfo::get_singleton()->get_data()->get_game_parameter_root());
			break;
		}
		case WwiseObjectType::Trigger:
		{
			root_element = build_tree("Triggers", WwiseProjectInfo::get_singleton()->get_data()->get_trigger_root());
			break;
		}
		case WwiseObjectType::AcousticTexture:
		{
			root_element = build_tree(
					"Virtual Acoustics", WwiseProjectInfo::get_singleton()->get_data()->get_acoustic_texture_root());
			break;
		}
		default:
			root_element->init(tree, nullptr, "WwiseGodot: Error", String(), WwiseObjectType::None);
			break;
	}

	return root_element;
}

AkWwiseTreeItem* AkWwiseTreeDataSource::build_tree(const String& p_name, const Array& p_wwise_tree_objects)
{
	AkWwiseTreeItem* root_folder = memnew(AkWwiseTreeItem);
	root_folder->init(tree, nullptr, p_name, String(), WwiseObjectType::PhysicalFolder);
	return build_tree(root_folder, p_wwise_tree_objects);
}

AkWwiseTreeItem* AkWwiseTreeDataSource::build_tree(AkWwiseTreeItem* p_tree_item, const Array& p_children_info)
{
	if (p_children_info.is_empty())
	{
		return p_tree_item;
	}

	int64_t child_info_size = p_children_info.size();
	for (int64_t i = 0; i < child_info_size; i++)
	{
		Ref<WwiseTreeObject> child_info = p_children_info[i];
		if (child_info.is_valid())
		{
			AkWwiseTreeItem* child_item = memnew(AkWwiseTreeItem);
			child_item->init(nullptr, p_tree_item->tree_item, child_info->get_name(), child_info->get_guid(),
					(WwiseObjectType)child_info->get_type());
			child_item = build_tree(child_item, child_info->get_children());

			if (child_item)
			{
				p_tree_item->add_wwise_item_child(child_item);
			}
		}
	}

	return p_tree_item;
}