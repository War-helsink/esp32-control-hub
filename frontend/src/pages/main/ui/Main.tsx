import { useState } from "react";
import { Button } from "@/shared/ui";

export const Main: React.FC = () => {
	const [result, setResult] = useState("None");

	const toggle = async () => {
		await fetch("/api/device")
			.then((res) => res.json())
			.then((json) => setResult(`${json.device} : ${json.status}`));
	};

	return (
		<main className="h-svh w-svw flex flex-col gap-6 justify-center items-center">
			<Button onClick={toggle}>Send</Button>
			{result}
		</main>
	);
};
