import type { NavBlockType, NavMenu } from "./types";

export function isNavBlock(item: NavMenu): item is NavBlockType {
	return "items" in item;
}
