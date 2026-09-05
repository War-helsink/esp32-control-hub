import { StrictMode } from "react";
import { createRoot } from "react-dom/client";
import { RouterProvider } from "react-router";
import { Toaster } from "@/shared/ui";
import { appRouter } from "./appRouter";
import "./index.css";

const LOADER_MIN_DURATION = 1500;

createRoot(document.getElementById("root")!).render(
	<StrictMode>
		<RouterProvider router={appRouter} />
		<Toaster position="top-center" />
	</StrictMode>,
);

const loader = document.getElementById("packet-bat-loader");

setTimeout(() => {
	if (!loader) return;

	loader.classList.add("hidden");

	loader.addEventListener(
		"transitionend",
		() => {
			loader.remove();
		},
		{ once: true },
	);
}, LOADER_MIN_DURATION);
