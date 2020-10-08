extends Node

export(String) var group_tag
export(NodePath) var ak_event:NodePath
export(String) var switch_group:String
export(String) var switch_value:String
export(AkUtils.GameEvent) var trigger_on:int = AkUtils.GameEvent.NONE
