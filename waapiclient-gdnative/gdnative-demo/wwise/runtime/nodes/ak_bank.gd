extends Node

export(String) var group_tag
export(AK.BANKS._enum) var bank:int
export(AkUtils.GameEvent) var load_on:int = AkUtils.GameEvent.NONE
export(AkUtils.GameEvent) var unload_on:int = AkUtils.GameEvent.NONE
