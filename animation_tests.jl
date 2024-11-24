### A Pluto.jl notebook ###
# v0.19.27

using Markdown
using InteractiveUtils

# ╔═╡ d80a2bb0-6794-11ee-11e2-b93a98daa5f8
begin
	import Pkg; Pkg.activate();
	using Makie; using GLMakie;
end

# ╔═╡ e98d2db5-26bc-4105-b27f-416d77a5d786
begin
	scene = Scene(backgroundcolor=:white, clear=true)

	cam3d!(scene)

	grass =  Circle(Point2f(0.0,0.0), 1)
	mesh!(scene, grass, color=:green)
	center!(scene)

	border = Circle(Point2f(0.0,0.0), 1)

	rotate!(border,  Vec2f(0,1,0), 0.5)
	
	
	
	
	
	scene
	
	
end

# ╔═╡ Cell order:
# ╠═d80a2bb0-6794-11ee-11e2-b93a98daa5f8
# ╠═e98d2db5-26bc-4105-b27f-416d77a5d786
