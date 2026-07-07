import type { LucideProps } from "lucide-react";

export type NavMenu = NavItemType | NavBlockType;

export interface NavItemType {
	readonly title: string;
	readonly url: string;
	readonly icon: React.ForwardRefExoticComponent<
		Omit<LucideProps, "ref"> & React.RefAttributes<SVGSVGElement>
	>;
}

export interface NavBlockType {
	readonly title: string;
	readonly items: NavItemType[];
}
