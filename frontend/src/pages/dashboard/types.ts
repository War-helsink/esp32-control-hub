export interface FeatureModule {
	id: string;
	title: string;
	description: string;
	extendedDoc: string;
	status: "ready" | "in_development" | "planned";
	icon: React.ReactNode;
}
