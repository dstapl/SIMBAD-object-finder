### A Pluto.jl notebook ###
# v0.19.47

using Markdown
using InteractiveUtils

# This Pluto notebook uses @bind for interactivity. When running this notebook outside of Pluto, the following 'mock version' of @bind gives bound variables a default value (instead of an error).
macro bind(def, element)
    quote
        local iv = try Base.loaded_modules[Base.PkgId(Base.UUID("6e696c72-6542-2067-7265-42206c756150"), "AbstractPlutoDingetjes")].Bonds.initial_value catch; b -> missing; end
        local el = $(esc(element))
        global $(esc(def)) = Core.applicable(Base.get, el) ? Base.get(el) : iv(el)
        el
    end
end

# ╔═╡ 9ca58c70-5bd9-11ee-145e-219a239f1062
begin
	using Pkg; Pkg.activate();
	using CSV, DataFrames, Parameters, Dates;
	using Distributed, DataFrames, VOTables;
	using EzXML;
	using EzXML: Document, Node;
	using PlutoUI, HypertextLiteral;

	const nameₗᵢₛₜ::Vector{String} = [
		# Names
		"MAIN_ID"
		"OTYPE_S"

		# Values
		"RA_d"
		"DEC_d"
		
		# # Motion values
		"PM:pmra"
		"PM:pmde"
		

		"GALDIM_MAJAXIS"
		"GALDIM_MINAXIS"
		# #"GALDIM_ANGLE"
			
		
		# #"FILTER_NAME_V"
		"FLUX_V"
		# #"FLUX_SYSTEM_V"
		
		# # "Distance:distance"
		# # "Distance:unit"

		# # Errors
		# #"PM:me_pmra"
		# #"PM:me_pmde"
		# "PM_ERR_MAJA"
		# "PM_ERR_MINA"
		# #"PM_ERR_ANGLE"
		
		# # Have mainly missing values so comment out
		# # "Distance:merr"
		# # "Distance:perr"

		# # Metadata
		# #"NB_REF"
		"OID4" # Identifier for SIMBAD
	];
	
	# # # Moved to config variables
	# # const curr_JD = Dates.datetime2julian(Dates.DateTime(Dates.today()));
	# # # Moved to constant variables
	# # Latitude of Hick's building, where the telescopes are located
	# const ϕ::Float64 = 53.38093176043183;
	# # Longitude ''
	# const σ::Float64 = -1.486126184463501;
	# # Elevation '' (from street level)
	# const Elevation::Float64 = 115 + 12; # Street elevation + building height
	# # Annual average local pressure (mB)
	# const P::Float64 = 0.5 * (1028.4 + 993.7)
	# # Average annual local temperature (°C)
	# const T::Float64 = 0.5 * (1027.1 + 1006)
	# # Slope of the observing surface measured from horizontal plane
	# const ω::Float64 = 0; # Assuming Hick's roof is flat/level
	# # Surface azimuth rotation angle
	# const γ::Float64 = 0; # Ditto
	# # Sun elevation, given sun radius is 0.26667°, atmos refrac is 0.5667 (In degrees)
	# const h′₀::Float64 = -0.8333
	# # Atmospheric Extinction from the telescope
	# const Aᵥ::Float64 = 0.52

	
	# 12:00 Noon 1st Jan 2000
	const J2000::Float64 = 2_451_545;
	# Mean longitude of the Sun at J2000
	const L₂₀₀₀::Float64 = 280.46646; 
	# Mean anomaly of the Sun at J2000
	const M₂₀₀₀::Float64 = 357.528;
	# Orbital eccentricity of Earth
	const Eₑ::Float64 = 0.016_71;
	# Obliquity of Earth's orbit at J2000
	const ϵₑ::Float64 = 23.43928; # °

	const L_values::Vector{Matrix{Float64}} = [
		[
		0 175347046 0 0
		1 3341656 4.6692568 6283.07585
		2 34894 4.6261 12566.1517
		3 3497 2.7441 5753.3849
		4 3418 2.8289 3.5231
		5 3136 3.6277 77713.7715
		6 2676 4.4181 7860.4194
		7 2343 6.1352 3930.2097
		8 1324 0.7425 11506.7698
		9 1273 2.0371 529.691
		10 1199 1.1096 1577.3435
		11 990 5.233 5884.927
		12 902 2.045 26.298
		13 857 3.508 398.149
		14 780 1.179 5223.694
		15 753 2.533 5507.553
		16 505 4.583 18849.228
		17 492 4.205 775.523
		18 357 2.92 0.067
		19 317 5.849 11790.629
		20 284 1.899 796.298
		21 271 0.315 10977.079
		22 243 0.345 5486.778
		23 206 4.806 2544.314
		24 205 1.869 5573.143
		25 202 2.458 6069.777
		26 156 0.833 213.299
		27 132 3.411 2942.463
		28 126 1.083 20.775
		29 115 0.645 0.98
		30 103 0.636 4694.003
		31 102 0.976 15720.839
		32 102 4.267 7.114
		33 99 6.21 2146.17
		34 98 0.68 155.42
		35 86 5.98 161000.69
		36 85 1.3 6275.96
		37 85 3.67 71430.7
		38 80 1.81 17260.15
		39 79 3.04 12036.46
		40 75 1.76 5088.63
		41 74 3.5 3154.69
		42 74 4.68 801.82
		43 70 0.83 9437.76
		44 62 3.98 8827.39
		45 61 1.82 7084.9
		46 57 2.78 6286.6
		47 56 4.39 14143.5
		48 56 3.47 6279.55
		49 52 0.19 12139.55
		50 52 1.33 1748.02
		51 51 0.28 5856.48
		52 49 0.49 1194.45
		53 41 5.37 8429.24
		54 41 2.4 19651.05
		55 39 6.17 10447.39
		56 37 6.04 10213.29
		57 37 2.57 1059.38
		58 36 1.71 2352.87
		59 36 1.78 6812.77
		60 33 0.59 17789.85
		61 30 0.44 83996.85
		62 30 2.74 1349.87
		63 25 3.16 4690.48 
		 ], # L0
		[
		0 628331966747 0 0
		1 206059 2.678235 6283.07585
		2 4303 2.6351 12566.1517
		3 425 1.59 3.523
		4 119 5.796 26.298
		5 109 2.966 1577.344
		6 93 2.59 18849.23
		7 72 1.14 529.69
		8 68 1.87 398.15
		9 67 4.41 5507.55
		10 59 2.89 5223.69
		11 56 2.17 155.42
		12 45 0.4 796.3
		13 36 0.47 775.52
		14 29 2.65 7.11
		15 21 5.34 0.98
		16 19 1.85 5486.78
		17 19 4.97 213.3
		18 17 2.99 6275.96
		19 16 0.03 2544.31
		20 16 1.43 2146.17
		21 15 1.21 10977.08
		22 12 2.83 1748.02
		23 12 3.26 5088.63
		24 12 5.27 1194.45
		25 12 2.08 4694
		26 11 0.77 553.57
		27 10 1.3 6286.6
		28 10 4.24 1349.87
		29 9 2.7 242.73
		30 9 5.64 951.72
		31 8 5.3 2352.87
		32 6 2.65 9437.76
		33 6 4.67 4690.48
		], # L1
		[
		0 52919 0 0
		1 8720 1.0721 6283.0758
		2 309 0.867 12566.152
		3 27 0.05 3.52
		4 16 5.19 26.3
		5 16 3.68 155.42
		6 10 0.76 18849.23
		7 9 2.06 77713.77
		8 7 0.83 775.52
		9 5 4.66 1577.34
		10 4 1.03 7.11
		11 4 3.44 5573.14
		12 3 5.14 796.3
		13 3 6.05 5507.55
		14 3 1.19 242.73
		15 3 6.12 529.69
		16 3 0.31 398.15
		17 3 2.28 553.57
		18 2 4.38 5223.69
		19 2 3.75 0.98
		], # L2
		[
		0 289 5.844 6283.076
		1 35 0 0
		2 17 5.49 12566.15
		3 3 5.2 155.42
		4 1 4.72 3.52
		5 1 5.3 18849.23
		6 1 5.97 242.73 
		], # L3
		[
		0 114 3.142 0
		1 8 4.13 6283.08
		2 1 3.84 12566.15 
		], # L4
		 [
		0 1 3.14 0
		 ] #L5
	]
	const B_values::Vector{Matrix{Float64}} = [
		[
			0 280 3.199 84334.662
			1 102 5.422 5507.553
			2 80 3.88 5223.69
			3 44 3.7 2352.87
			4 32 4 1577.34 
		], # B0
		[
			0 9 3.9 5507.55
			1 6 1.73 5223.69
		] # B1
	]
	const R_values::Vector{Matrix{Float64}} = [
		[
			0 100013989 0 0
			1 1670700 3.0984635 6283.07585
			2 13956 3.05525 12566.1517
			3 3084 5.1985 77713.7715
			4 1628 1.1739 5753.3849
			5 1576 2.8469 7860.4194
			6 925 5.453 11506.77
			7 542 4.564 3930.21
			8 472 3.661 5884.927
			9 346 0.964 5507.553
			10 329 5.9 5223.694
			11 307 0.299 5573.143
			12 243 4.273 11790.629
			13 212 5.847 1577.344
			14 186 5.022 10977.079
			15 175 3.012 18849.228
			16 110 5.055 5486.778
			17 98 0.89 6069.78
			18 86 5.69 15720.84
			19 86 1.27 161000.69
			20 65 0.27 17260.15
			21 63 0.92 529.69
			22 57 2.01 83996.85
			23 56 5.24 71430.7
			24 49 3.25 2544.31
			25 47 2.58 775.52
			26 45 5.54 9437.76
			27 43 6.01 6275.96
			28 39 5.36 4694
			29 38 2.39 8827.39
			30 37 0.83 19651.05
			31 37 4.9 12139.55
			32 36 1.67 12036.46
			33 35 1.84 2942.46
			34 33 0.24 7084.9
			35 32 0.18 5088.63
			36 32 1.78 398.15
			37 28 1.21 6286.6
			38 28 1.9 6279.55
			39 26 4.59 10447.39
		], # R0
		[
			0 103019 1.10749 6283.07585
			1 1721 1.0644 12566.1517
			2 702 3.142 0
			3 32 1.02 18849.23
			4 31 2.84 5507.55
			5 25 1.32 5223.69
			6 18 1.42 1577.34
			7 10 5.91 10977.08
			8 9 1.42 6275.96
			9 9 0.27 5486.78
		], # R1
		[
			0 4359 5.7846 6283.0758
			1 124 5.579 12566.152
			2 12 3.14 0
			3 9 3.63 77713.77
			4 6 1.87 5573.14
			5 3 5.47 18849.23
		], # R2
		[
			0 145 4.273 6283.076
			1 7 3.92 12566.15
		], # R3
		[
			0 4 2.56 6283.08
		] # R4
	]

	const Y_values::Matrix{Float64} = [
		0 0 0 0 1
		-2 0 0 2 2 
		0 0 0 2 2 
		0 0 0 0 2 
		0 1 0 0 0 
		0 0 1 0 0 
		-2 1 0 2 2 
		0 0 0 2 1 
		0 0 1 2 2 
		-2 -1 0 2 2 
		-2 0 1 0 0 
		-2 0 0 2 1 
		0 0 -1 2 2 
		2 0 0 0 0
		0 0 1 0 1
		2 0 -1 2 2
		0 0 -1 0 1
		0 0 1 2 1 
		-2 0 2 0 0 
		0 0 -2 2 1 
		2 0 0 2 2 
		0 0 2 2 2 
		0 0 2 0 0
		-2 0 1 2 2 
		0 0 0 2 0
		-2 0 0 2 0 
		0 0 -1 2 1
		0 2 0 0 0 
		2 0 -1 0 1 
		-2 2 0 2 2 
		0 1 0 0 1 
		-2 0 1 0 1 
		0 -1 0 0 1
		0 0 2 -2 0
		2 0 -1 2 1
		2 0 1 2 2 
		0 1 0 2 2
		-2 1 1 0 0 
		0 -1 0 2 2 
		2 0 0 2 1
		2 0 1 0 0 
		-2 0 2 2 2 
		-2 0 1 2 1 
		2 0 -2 0 1
		2 0 0 0 1
		0 -1 1 0 0 
		-2 -1 0 2 1
		-2 0 0 0 1
		0 0 2 2 1 
		-2 0 2 0 1
		-2 1 0 2 1
		0 0 1 -2 0
		-1 0 1 0 0
		-2 1 0 0 0
		1 0 0 0 0
		0 0 1 2 0
		0 0 -2 2 2
		-1 -1 1 0 0
		0 1 1 0 0
		0 -1 1 2 2
		2 -1 -1 2 2
		0 0 3 2 2
		2 -1 0 2 2
	]

	const ψϵ_values::Matrix{Float64} = [
		-171996 -174.2 92025 8.9 
		-13187 -1.6 5736 -3.1 
		-2274 -0.2 977 -0.5 
		2062 0.2 -895 0.5 
		1426 -3.4 54 -0.1 
		712 0.1 -7 0 # 4
		-517 1.2 224 -0.6 
		-386 -0.4 200 0 # 4
		-301 0 129 -0.1 # 2 
		217 -0.5 -95 0.3 
		-158 0 0 0 # 2 3 4
		129 0.1 -70 0 # 4
		123 0 -53 0 # 2 4
		63 0 0 0 #  2 3 4
		63 0.1 -33 0 # 4
		59 0 26 0 # 2 4
		-58 -0.1 32 0 # 4
		-51 0 27 0 # 2 4
		48 0 0 0 # 2 3 4
		46 0 -24 0 # 2 4
		-38 0 16 0 # 2 4
		-31 0 13 0 # 2 4
		29 0 0 0 # 2 3 4
		29 0 -12 0 # 2 4
		26 0 0 0 # 2 3 4
		-22 0 0 0 # 2 3 4
		21 0 -10 0 # 2 4
		17 -0.1 0 0 # 3 4
		16 0 -8 0 # 2 4
		-16 0.1 7 0 # 4
		-15 0 9 0 # 2 4
		-13 0 7 0 # 2 4
		-12 0 6 0 # 2 4
		11 0 0 0 # 2 3 4
		-10 0 5 0 # 2 4
		-8 0 3 0 # 2 4
 		7 0 -3 0 # 2 4
		-7 0 0 0 # 2 3 4
		-7 0 3 0 # 2 4
		-7 0 3 0 # 2 4
		6 0 0 0 # 2 3 4
		6 0 -3 0 # 2 4
		6 0 -3 0 # 2 4
		-6 0 3 0 # 2 4
		-6 0 3 0 # 2 4
		5 0 0 0 # 2 3 4
		-5 0 3 0 # 2 4
		-5 0 3 0 # 2 4
		-5 0 3 0 # 2 4
		4 0 0 0 # 2 3 4
		4 0 0 0 # 2 3 4
		4 0 0 0 # 2 3 4
		-4 0 0 0 # 2 3 4
		-4 0 0 0 # 2 3 4
		-4 0 0 0 # 2 3 4
		3 0 0 0 # 2 3 4
		-3 0 0 0 # 2 3 4
		-3 0 0 0 # 2 3 4
		-3 0 0 0 # 2 3 4
		-3 0 0 0 # 2 3 4
		-3 0 0 0 # 2 3 4
		-3 0 0 0 # 2 3 4
		-3 0 0 0 # 2 3 4

	]
	
	md"###### Imports & Constants"
end

# ╔═╡ 01d07651-2162-4b9d-b88a-be2002fc309b
begin
	global constants
	# 	# const curr_JD = Dates.datetime2julian(Dates.DateTime(Dates.today()));
	# # Latitude of Hick's building, where the telescopes are located
	# const ϕ::Float64 = 53.38093176043183;
	# # Longitude ''
	# const σ::Float64 = -1.486126184463501;
	# # Elevation '' (from street level)
	# const Elevation::Float64 = 115 + 12; # Street elevation + building height
	# # Annual average local pressure (mB)
	# const P::Float64 = 0.5 * (1028.4 + 993.7)
	# # Average annual local temperature (°C)
	# const T::Float64 = 0.5 * (1027.1 + 1006)
	# # Slope of the observing surface measured from horizontal plane
	# const ω::Float64 = 0; # Assuming Hick's roof is flat/level
	# # Surface azimuth rotation angle
	# const γ::Float64 = 0; # Ditto
	# # Sun elevation, given sun radius is 0.26667°, atmos refrac is 0.5667 (In degrees)
	# const h′₀::Float64 = -0.8333
	# # Atmospheric Extinction from the telescope (Use average seeing value)
	# const Aᵥ::Float64 = 0.52
	@bind constants confirm(PlutoUI.combine() do Child 
		md"""
		# Constants
		
		- Latitude ϕ: $(Child(NumberField(-90:90, default=53.380)))
		- Longitude σ: $(Child(NumberField(-180:180, default=-1.486)))
		- Elevation [m]: $(Child(NumberField(0:typemax(Int64), default=100)))
		- Annual average local pressure [mB]: $(Child(NumberField(0:typemax(Int64), default=1013.25)))
		- Average annual local temperature [°C]: $(Child(NumberField(-273.15:100, default=20)))
		- Slope of observing surface (from horizontal): $(Child(NumberField(-90:90, default=0)))
		- Surface azimuth rotation angle (from horizontal): $(Child(NumberField(-90:90, default=0)))
		- Sun elevation [°] (R⊙=0.2667°, atmos refrac = 0.5667): $(Child(NumberField(-90:90, default=-0.8333)))
		- Atmospheric Extinction (Use average seeing value): $(Child(NumberField(0:typemax(Int64), default=0.52)))
		"""
	end)
end

# ╔═╡ bc9195bd-9cf8-4f9f-bb7a-cbc4f0359576
begin
	global ϕ, σ, Elevation, P, T, ω, γ, h′₀, Aᵥ;

	# Latitude
	ϕ::Float64 = constants[1];
	# Longitude ''
	σ::Float64 = constants[2];
	# Elevation '' (from street level)
	Elevation::Float64 = constants[3];
	# Annual average local pressure (mB)
	P::Float64 = constants[4];
	# Average annual local temperature (°C)
	T::Float64 = constants[5];
	# Slope of the observing surface measured from horizontal plane
	ω::Float64 = constants[6]; 
	# Surface azimuth rotation angle
	γ::Float64 = constants[7];
	# Sun elevation, given sun radius is 0.26667°, atmos refrac is 0.5667 (In degrees)
	h′₀::Float64 = constants[8];
	# Atmospheric Extinction from the telescope (Use average seeing value)
	Aᵥ::Float64 = constants[9];
	md"###### Constant bindings"
end

# ╔═╡ 67da4a5c-9925-42a1-a9f5-b603644567f3
begin
	# Julian Day
	const ΔT::Float64 = 69.3752 # seconds, updated 1st Feb 2020
	function JD(Y::UInt16,M::UInt16,D::UInt16,h::UInt8=0,m::UInt8=0,s::UInt8=0)::Float64
		local A, B, jd
		# B = 0W for Julian, (2 - A + trunc(A/4), where A = trunc(Y/100)
		A = trunc(Y/100)
		B = 2 - A + trunc(A/4)
		jd =  trunc(365.25*(Y+4716)) + trunc(30.6001 * (M+1)) + D + B - 1524.5
		jd + ( h - 12 ) / 24 + m / 1440 + s / 86400
	end

	# Julian Ephemeris Day
	function JDE(jd::Float64)::Float64
		jd + ΔT / 86400
	end
	# Julian year
	function JY(jd::Float64)::Float64
		(jd - J2000) / 365.25
	end
	# Ditto ephemeris
	function JYE(jd::Float64)::Float64
		(JDE(jd) - J2000) / 365.25
	end
	# Julian century
	function JC(jd::Float64)::Float64
		JY(jd) / 100
		# (jd - J2000) / 36525
	end
	# Ditto ephemeris
	function JCE(jd::Float64)::Float64
		JYE(jd) / 100
		# (JDE(jd) - J2000) / 36525
	end
	# ditto millennium
	function JME(jd::Float64)::Float64
		JCE(jd) / 10
	end

	
	
	# function constructValue(T::Vector{Matrix{Float64}},jd::Float64)
	# 	curr_JME = JME(jd)
		
	# 	T_length::UInt8 = size(T,2)
	# 	T_builder = [Vector{Float64}(undef,5) for _ in 0:T_length]
	# 	display(typeof(T_builder))
	# 	for n ∈ 1:(T_length+1) # 0:5 -> 1:6
	# 		# T[n][i] = A[n][i] * cos(B[n][i] + C[n][i] * curr_JME)
	# 		A = view(T[n], :, 2)
	# 		display(typeof(A))
	# 		B = view(T[n], :, 3)
	# 		C = view(T[n], :, 4)
	# 		T_builder[n] = map((a,b,c) -> (a * cos(b + c * curr_JME)), A, B, C)
	# 	end
	
	# 	T_sums = map(sum, T_builder)
	# 	T = 10^-8 * (@evalpoly curr_JME T_sums...)
	# end

	# # const curr_JDE = JDE(curr_JD)
	# # const curr_JC = JC(curr_JD)
	# # const curr_JCE = JCE(curr_JD)
	# # const curr_JME = JME(curr_JD)

	# # Heliocentric longitude
	# function L(jd)
	# 	mod(
	# 		rad2deg(
	# 			constructValue(L_values, jd)
	# 		),
	# 		360
	# 	)
	# end

	# # Heliocentric lattitude
	# function B(jd)
	# 	rad2deg(
	# 		constructValue(B_values, jd)
	# 	)
	# end

	# # Earth radius vector (Magnitude)
	# function R(jd)
	# 	rad2deg(
	# 		constructValue(R_values, jd)
	# 	)
	# end
	# md"Constructing resultant values from end Table - Only runs once"
	
	# # Geocentric longitude
	# function Θ(jd)
	# 	mod(L(jd) + 180, 360)
	# end 
	
	# # Geocentric lattitude
	# β(jd) = -B(jd)

	# function X⃗(jd)
	# 	curr_JCE = JCE(jd)
		
	# 	# Mean elongation of the moon from the sun
	# 	X₀ = @evalpoly curr_JCE 297.85036 445267.111480 -0.0019142 1/189474
		
	# 	# Mean anomaly of the sun (*Earth)
	# 	X₁ = @evalpoly curr_JCE 357.52772 35999.050340 -0.0001603 -1/300000
	
	# 	# Mean anomaly of the moon
	# 	X₂ = @evalpoly curr_JCE 134.96298 477198.867398 0.0086972 1/56250
		
	# 	# Argument of latitude (Moon)
	# 	X₃ = @evalpoly curr_JCE 93.27191 483202.017538 -0.0036825 1/327270
	
	# 	# Longitude of ascending node of the moon's nean orbit (On Ecliptic, measured from mean Equinox on curr date)
	# 	X₄ = @evalpoly curr_JCE 125.04452 -1934.136261 0.0020708 1/450000
	
	# 	mod.([X₀, X₁, X₂, X₃, X₄], 360)
	# end

	
	# # Nutation in longitude Δψ, nutation in obliquity Δϵ
	# # Δψᵢ = (aᵢ + bᵢ * JCE) * sin(∑ⱼ₌₀₋₄ Xⱼ * Yᵢ,ⱼ)
	# # Δϵᵢ = (cᵢ + dᵢ * JCE) * cos(∑ⱼ₌₀₋₄ Xⱼ * Yᵢ,ⱼ)

	# function Δψ(jd)
	# 	curr_JCE = JCE(jd)
		
	# 	arr_length = size(Y_values,1) # Same as ψϵ_values
	# 	ψ_builder = fill(0., (arr_length, 1))
	# 	X_vector = X⃗(curr_JD)
		
	# 	a = view(ψϵ_values, :, 1)
	# 	b = view(ψϵ_values, :, 2)
		
	# 	for i ∈ 1:(arr_length) # 0:5 -> 1:6	
	# 		XY_sum = sum(X_vector[j] * Y_values[i,j] for j ∈ 1:5) # 0:4 -> 1:5
	# 		ψ_builder[i,1] = (a[i] + b[i]*curr_JCE) * sin(XY_sum)  
	# 	end
		
	# 	Δψ_sum = sum(ψ_builder)

	# 	Δψ_sum / 36000000
	# end

	# function Δϵ(jd)
	# 	curr_JCE = JCE(jd)
		
	# 	arr_length = size(Y_values,1) # Same as ψϵ_values
	# 	ϵ_builder = fill(0., (arr_length, 1))
	# 	X_vector = X⃗(curr_JD)
		
	# 	c = view(ψϵ_values, :, 3)
	# 	d = view(ψϵ_values, :, 4)
		
	# 	for i ∈ 1:(arr_length) # 0:5 -> 1:6	
	# 		XY_sum = sum(X_vector[j] * Y_values[i,j] for j ∈ 1:5) # 0:4 -> 1:5
	# 		ϵ_builder[i,1] = (c[i] + d[i]*curr_JCE) * cos(XY_sum)  
	# 	end

	# 	Δϵ_sum = sum(ϵ_builder)

	# 	Δϵ_sum / 36000000
	# end# 13.45% and 67.7% errors


	# # Mean obliquity of the ecliptic in arcseconds
	# function ϵ(jd)
	# 	curr_JME = JME(jd)
		
	# 	ϵ₀ = @evalpoly curr_JME/10 84381.448 -4680.93 -1.55 1999.25 -51.38 -249.67 -39.05 7.12 27.87 5.79 2.45
	
	# 	# True obliquity of the ecliptic in degrees
	# 	ϵ₀ / 3600 + Δϵ(jd)
	# end

	# function λ(jd)
		
	# 	# Abberation correction in degrees
	# 	Δτ =  -20.4898/(3600 * R(jd))
	
	# 	# Apparent sun longitude in degrees
	# 	Θ(jd) + Δψ(jd) + Δτ
	# end

	# function v(jd)
	# 	curr_JC = JC(jd)	
	# 	# Mean sidereal time at Greenwich (in degrees)
	# 	v₀ = 280.46061837 + 360.98564736629*(jd - J2000) +  @evalpoly curr_JC 0 0 0.000387933 -1/38710000
	# 	v₀ = mod(v₀, 360)
		
	# 	# Apparent sidereal time at Greenwich (in degrees)
	# 	v₀ + Δψ(jd) * cos(ϵ(jd))
	# end

	# function α(jd)
	# 	l = λ(jd)
	# 	b = β(jd)
	# 	e = ϵ(jd)
	# 	# Geocentric sun right ascension (in degrees)
	# 	a = atand(
	# 		sind(l)*cosd(e) - tand(b)*sind(e), 
	# 		cosd(l)
	# 	)
	# 	mod(a, 360)
	# end

	# function δ(jd)
	# 	l = λ(jd)
	# 	b = β(jd)
	# 	e = ϵ(jd)
		
	# 	# Geocentric sun declination in degrees
	# 	asind(sind(b)*cosd(e) + cosd(b)*sind(e)*sind(l))
	# end

	# function H(jd)
	# 	# Observer local hour angle (In degrees)
	# 	mod(v(jd) + σ - α(jd), 360)	
	# end

	# function Δα(jd)
	# 	# Topocentric sun right ascension (In degrees)
	# 	ξ = 8.794/(3600*R(jd))
	
	# 	# Term u (In radians)
	# 	u = atan(0.99664719*tan(ϕ))
	
	# 	# Term x
	# 	x = cos(u) + cosd(ϕ)*Elevation/6378140
	
	# 	# Term y
	# 	y = 0.99664719*sin(u) + sind(ϕ)*Elevation/6378140
		
	# 	l = λ(jd)
	# 	b = β(jd)
	# 	e = ϵ(jd)
	# 	h = H(jd)
		
	# 	# Parallax in sun right ascension
	# 	atand(
	# 		-x*sind(ξ)*sind(h), 
	# 		cosd(δ(jd)) - x*sind(ξ)*cosd(h)
	# 	)
	# end
	
	# function α′(jd)
	# 	# Topocentric sun right ascension
	# 	α(jd) + Δα(jd)
	# end

	# function δ′(jd)
	# 	# Topocentric sun right ascension (In degrees)
	# 	ξ = 8.794/(3600*R(jd))
	
	# 	# Term u (In radians)
	# 	u = atan(0.99664719*tan(ϕ))
	
	# 	# Term x
	# 	x = cos(u) + cosd(ϕ)*Elevation/6378140
	
	# 	# Term y
	# 	y = 0.99664719*sin(u) + sind(ϕ)*Elevation/6378140
		
	# 	d = δ(jd)
		
	# 	# Topocentric sun declination
	# 	atand(
	# 		(sind(d) - y*sind(ξ)*cosd(Δα(jd))),
	# 		cosd(d) - x*sind(ξ)*cosd(H(jd))
	# 	)
		
	# end

	# function H′(jd)
	# 	# Topocentric local hour angle
	# 	H(jd) - Δα(jd)
	# end

	# function θ(jd)
	# 	d′ = δ′(jd)
	# 	h′ = H′(jd)
		
	# 	# Topocentric elevation angle w/o atmospheric refraction correction
	# 	e₀ = asind(sind(ϕ)*sind(d′) + cosd(ϕ)*cosd(d′)*cosd(h′))
	
	# 	# Atmospheric refraction correction
	# 	Δe = (P/1010) * (283/(274.15 + T)) * (102/(60 * tand(e₀ + 103/(e₀ + 5.11))))
	# 	# Δe = 0 when sun below horizon
	
	# 	# Topocentric elevation angle (in degrees)
	# 	e = e₀ + Δe
		
	# 	# Topocentric zenith angle (In degrees)
	# 	90 - e
	# end

	# function Γ(jd)
	# 	h′ = H′(jd)
	# 	d′ = δ′(jd)
		
	# 	# Topocentric astronomers azimuth angle (in degrees)
	# 	g = atand(
	# 		sind(h′),
	# 		cosd(h′)*sind(ϕ) - tand(d′)*cosd(ϕ)
	# 	)
	# 	mod(g, 360)
	# end

		
	# # Topocentric azimuth angle Φ
	# Φ(curr_JD) = mod(Γ(curr_JD) + 180, 360)

	# function I(curr_JD)
	# 	t = θ(curr_JD)
	# 	g = Γ(curr_JD)
		
	# 	# Incidence angle for a surface oriented in any direction (in degrees)
	# 	acosd(
	# 		cosd(t)*cosd(ω) + sind(ω)*sind(t)*cosd(g-γ)
	# 	)
	# end

	
	# function M(curr_JD)
	# 	curr_JME = JME(curr_JD)
	# 	# Sun's mean longitude (In degrees)
	# 	m = @evalpoly curr_JME 280.4664567 360007.6982779 0.03032028 1/49931 -1/15300 -1/2000000
	# 	mod(m, 360)
	# end
	
	# # Equation of Time (degrees)
	# E(curr_JD) = M(curr_JD) - 0.0057173 - α(curr_JD) + Δψ(curr_JD) * cosd(ϵ(curr_JD))

	# function ERA(jd)
	# 	tᵤ = jd - J2000
	# 	rad2deg(2π*(0.7790572732640 + 1.00273781191135448 * tᵤ))
	# end
	# function precession(jd)
	# 	a = α(jd)
	# 	d = δ(jd)

	# 	T = JC(jd) * 10^-6
	# 	# In "/julian year
	# 	p₊ = @evalpoly T 50.475838 -27.000654 15.603265
		
	# 	# m = M(curr_JD)
	# 	# n = (curr_JD)
	# 	# Δα = M + N*sind(α)*tand(δ)
	# 	# Δδ = N*cosd(α)
	# 	# (Δα,Δδ)
	# end

	# # Sidereal time
	# GMST(jd) = mod(ERA(jd) - precession(jd), 360)
	# LMST(jd) = mod(GMST(jd) + ϕ, 360)

	md"Following https://www.nrel.gov/docs/fy08osti/34302.pdf"
end

# ╔═╡ 032f6250-cd49-462c-9363-0d818e7c8345
begin
	# Real, tuple/arr{Real,Real}, Real
	# function starBetween2(x, y, z)::Bool
	# 	if (y[2] < z) || (y[1] > z)

	# 		bool = !(y[2] < x < y[1])
	# 	else
	# 		bool =  y[1] < x < y[2]
	# 	end
	# 	bool
	# end
	
	function properMotion(curr_JD::Float64, (μₐ, μₖ)::Tuple{Float32,Float32})::Tuple{Float64, Float64} # Stupid symbols: pmra, pmdec (mas)
		if all(ismissing, (μₐ, μₖ))
			return (0,0)
		end
		date::Float64 = JY(curr_JD) #Messier.n(date)

		# Proper motion mas.yr⁻¹, in Julian years
		Δαₚₘ::Float64 = ismissing(μₐ) ? 0 : date * μₐ
		Δδₚₘ::Float64 = ismissing(μₖ) ? 0 : date * μₖ
		1/3600000 .* (Δαₚₘ, Δδₚₘ) # mas to degree
	end


	function quadrantα(α::Float64)::Float64
		mod(α,360)
	end
	function quadrantδ(δ::Float64)::Float64
		if -90 ≤ δ ≤ 90
			return δ
		end
		δ = mod(δ, 90)
		δ = mod(δ + 90, 360)
		δ > 45 ? δ - 90 : δ
		# (δ > 90 ? 1 : -1)*180 - δ
	end
	function correctCoords(curr_JD::Float64, α::Float64, δ::Float64, (μₐ, μₖ)::Tuple{Float32,Float32})::Matrix{Float64}
		(Δαₚₘ::Float64, Δδₚₘ::Float64) = properMotion(curr_JD, (μₐ, μₖ))
		(Δα::Float64, Δδ::Float64) = (0,0)
		
		#println((Δα,Δδ))
		# if solar
		# 	(Δα, Δδ) = precession(α, δ, date)
		# end
		# (Δαₚₘ, Δδₚₘ) = (0,0)
		# In degrees
		
		ϵₐ::Float64 = Δαₚₘ + Δα
		ϵₖ::Float64 = Δδₚₘ + Δδ
		[quadrantα(α + ϵₐ) quadrantδ(δ + ϵₖ)]
	end

	function between(x::T, y::Tuple{U,U}; inclusive::Bool = false)::Bool where {T<:Number, U<:Number}
		inclusive ?  y[1] <= x <= y[2] : y[1] < x < y[2];
	end

	# x::T, y::Union{Tuple{T,T}, Vector{T}}
	function between2(x::T, y::Tuple{U,U}; inclusive::Bool = false)::Bool where {T<:Number, U<:Number}
		operator = (<, <=)[inclusive+1]
		return if operator(y[2], y[1])
			!(operator(y[2],x) && operator(x, y[1]))
		else
			operator(y[1],x) && operator(x, y[2])
		end
		bool
	end
	
	function rbetween(x, y; inclusive = false)::Bool
		if all(!isnothing, y)
			return between2(x,y;inclusive)
		end
		idx = findfirst(isnothing, y)
		(<,>)[idx](x, y[3-idx]) # Why is it 3-idx??
	end
	
	rbetween(::Missing, _...)::Bool = true; 

	coaltitude(ϕ::Float64, δ::Float64=0)::Float64 = 90 - (ϕ - δ);

	function transitAlt(δ::Real)::Tuple{Float64,Bool}
		# Transit altitude is 90 - ϕ + δ, or if |δ - ϕ| > 90 ⟹ 180 - (90 - ϕ + δ)
		# ⟹ 90 - ϕ + δ, or -90 + ϕ - δ
		# ⟹ 90 + (δ - ϕ) or -90 + (ϕ - δ) ≡ -90 - (δ - ϕ)
		# ⟹ ± (90 + (δ - ϕ))
		Δ = δ - ϕ 
		# Angles greater than 90° ⟹ Northen transit (Irregular, lower culmination)
		# Angles less than 90° ⟹ Southern transit (Standard, upper culmination)
		
		altᵣ = 90 + (Δ > -90 ? 1 : -1)*Δ
		north = altᵣ > 90
		(north ? 90 - mod(altᵣ, 90) : mod(altᵣ, 90), north) # 1 indicates northen, 0 indicates southern
	end

	function deg_to_dms(x::Float64)::Tuple{Int32, Int32, Float64}
	    d::Int32, res::Float64 = divrem(x, 1.0)
	    m::Int32, res = divrem(res * 60.0, 1.0)
	    s::Float64 = res * 60.0
    	(d, m, s)
	end
	dms_to_deg(d::Int32, m::Int32, s::Float64)::Float64 = ((s / 60.0) + m) / 60.0 + d
	dms_to_deg(x::Tuple{Int32,Int32,Float64}) = dms_to_deg(x...)
	
	function dms_to_string(x::Tuple{Int32,Int32,Float64})::String
		"$(x[1])° $(x[2])' $(round(x[3]))'' "
	end

	function deg_to_hours(x::Float64)::Tuple{Int32, Int32, Float64}
		x /= 15
		h::Int32, res::Float64 = divrem(x, 1.0)
		m::Int32, res = divrem(res*60.0, 1.0)
		s::Float64 = res*60.0
		(h, m, s)
	end
	hours_to_deg(h::Int32, m::Int32, s::Float64)::Float64 = 15*(((s / 60.0) + m) / 60.0 + h)
	hours_to_deg(x::Tuple{Int32,Int32,Float64}) = hours_to_deg(x...)
	function hours_to_string(x::Tuple{Int32,Int32,Float64})::String
		"$(x[1])h $(x[2])m $(round(x[3]))s"
	end
	
	md"###### Helper functions"
end

# ╔═╡ abb151f6-481b-4bd2-8d06-1eeb21a66a89
begin
	# Simbad VOTable datatypes
	const uType = Union{Missing, Float32, Float64, Int16, Int32, UInt8, String, Int64};
	
	
	TB(source::AbstractString)::Tuple{Vector{String}, Vector{Vector{uType}}} = TB(parsexml(source)::Document)


	rowdata(::Type{<:AbstractArray{T}}, row) where {T} = safeparse.(eltype(T), split(row))
	rowdata(::Type{T}, row) where {T} = safeparse(T, row)

	# Handle missing values
	function safeparse(T::DataType, val::U)::Union{T,Missing} where {U}
		out = tryparse(T, val)
		isnothing(out) ? missing : out
	end

	# Convert XML types to Julia types
	TYPE_MAP::Dict{String, DataType} = Dict(
		"boolean" => Bool,
		"unsignedByte" => UInt8,
		"short" => Int16,
		"int" => Int32,
		"long" => Int64,
		"float" => Float32,
		"double" => Float64,
		"floatComplex" => ComplexF32,
		"doubleComplex" => ComplexF64,
	      # "bit" => ,
		"char" => UInt8,
		"unicodeChar" => Char
	)


	function safeparse(::Type{String}, val::T)::Union{T,Missing} where {T}
		isempty(val) ? missing : val
	end
    



	function TB(xml::Document)::Tuple{Vector{String}, Vector{Vector{uType}}}
		# validate input is VOTable
		xml_root = root(xml)
		
		if nodename(xml_root) != "VOTABLE"
			throw(ArgumentError("input XML does not appear to be a VOTable"))
		end

		ns::Vector{Pair{String, String}} = ["x" => namespace(xml_root)]
		
		#resources = findall("x:RESOURCE", root(xml), ns)

		Nodes::Vector{Node} = findall(
			"x:RESOURCE/x:TABLE",
			xml_root, 
			ns
		)

		
		tables_length = length(Nodes)
		tables = Vector{Tuple{Vector{String}, Vector{Vector{uType}}}}(undef, tables_length)

		@inbounds @simd for i in 1:tables_length
			tables[i] = TBdata(Nodes[i], ns)
		end
		
		
		return tables[1]
	end
	
	function TB(source::AbstractString)::Tuple{Vector{String}, Vector{Vector{uType}}}
		xml_parsed::Document = parsexml(source)
		TB(xml_parsed)
	end
	
	function TBdata(node::Node, ns::Vector{Pair{String, String}})::Tuple{Vector{String}, Vector{Vector{uType}}}
		data_nodes = findfirst("x:DATA/x:TABLEDATA", node, ns)

		fields::Vector{Node} = findall("x:FIELD", node, ns)

		names::Vector{String} = map(f -> f["name"], fields)

      	types::Vector{DataType} = map(fields) do field::Node
			atts = map(f -> nodename(f), attributes(field))
			datatype = field["datatype"]

			if "arraysize" ∈ atts
				if datatype == "char"
					return String
				  else
					return Array{TYPE_MAP[datatype]}
				end
			end
					
			return TYPE_MAP[datatype]
		end

		rows = Vector{Vector{uType}}(undef, 1)
		
		data_nodes_elements = elements(data_nodes)
		rows_size = length(data_nodes_elements)
		rows = Vector{Vector{uType}}(undef, rows_size)

		
		@inbounds @simd for i ∈ 1:rows_size
			curr_entry = elements(data_nodes_elements[i])
			
			curr_row_length = length(curr_entry)
			curr_row = Vector{uType}(undef, curr_row_length)

			for j ∈ 1:curr_row_length
				@inbounds curr_row[j] = rowdata(types[j], nodecontent(curr_entry[j]))	
			end
			
			rows[i] = curr_row
		end

	    (names, rows)
		
	end
	
	function openVOTABLE(filename)::DataFrame
		open(filename, "r") do file
				
			# Replacement for VOTable since it's so inconsistent
			file_contents = read(file, String) # Minimal time
			
			raw_data = TB(file_contents)

			
			namesₐₗₗ::Vector{Symbol} = Symbol.(raw_data[1])

			dataᵣ::Vector{Vector{uType}} = raw_data[2]
	
			
			# dataᵥ = Vector{Matrix{uType}}(undef, length(dataᵣ))
			# #println(typeof(data_values))
			# @time @sync @distributed for j in 1:length(dataᵣ)
			# 	dataᵥ[j] = permutedims(dataᵣ[j])
			# end

			dataᵥ::Vector{Matrix{uType}} = map(permutedims, dataᵣ)

			dataₘ = Matrix{uType}(undef, (size(dataᵥ,1),size(dataᵥ[1],2)))
			
			@simd for i ∈ 1:size(dataᵥ,1)
				@inbounds dataₘ[i,:] .= vec(dataᵥ[i])
			end

			dataset::DataFrame = DataFrame(dataₘ, namesₐₗₗ)

			dataset
			
		end;
	end

	md"###### Dataset Loading functions"
end

# ╔═╡ 9a21c39d-74ad-4c7f-aa40-25a95174a5ef
begin
	global filename
	md"""$(@bind filename FilePicker([MIME("text/")]))"""
end

# ╔═╡ 2841f14e-945f-40ef-a33f-404f8adf69b5
begin
	global config
	@bind config confirm(PlutoUI.combine() do Child 
		# - tₜᵣₐₙₛᵢₜ $(Child(TimePicker(default = Time(18,0,0)))) → $(Child(TimePicker(default = Time(7,0,0))))
		md"""
		# Config
		
		- Date: $(Child(DatePicker(default=Dates.today())))
		- FOV [%]: $(Child(Slider(0:100, default=0, show_value=true)))
		- |V|ₘᵢₙ: $(Child(Slider(23:-1:-2, default=23, show_value=true)))
		- n_stars (At |V|ₘᵢₙ): $(Child(NumberField(1:1000, default=1)))
		- α [h]: $(Child(Scrubbable(0:24,default=0, suffix="h"))) → $(Child(Scrubbable(0:24,default=24, suffix="h")))
		- δ [°]: $(Child(RangeSlider(-90:90;show_value=true, default=-90:90)))
		- altₘᵢₙ [°]: $(Child(Slider(0:90, default=0, show_value = true)))
		- ID Search: $(Child(TextField(default="", placeholder="Enter ID substring")))
		- OTYPE Search: $(Child(TextField(default="", placeholder="Enter Object Type")))
		- Hide Missing Magnitudes: $(Child(CheckBox(default = false)))
		- Sort by column: $(Child(Select( nameₗᵢₛₜ )))
		"""
	end)
end

# ╔═╡ e0a95451-ea45-4963-b224-bedbbfd1d2a2
if !isnothing(filename)
	global dataset
	dataset::DataFrame = openVOTABLE(filename["name"])
	md"""###### Load dataset ($(nrow(dataset)) objects)"""
else
	@htl("<b><font size=\"24\"><span style=\"color:#ff1c1c\">Select a file, in the cell above.</span></font></b>")
end

# ╔═╡ 47329fcd-1cdd-47e2-9dd1-7aeed35b7e28
begin
	global curr_JD, fov, Vₘᵢₙ, αᵣ, δᵣ, dimₘᵢₙ, dimₘₐₓ, altₘᵢₙ,n_stars, id_search, otype_search, hide_missing_vband;

	curr_JD::Float64 =  Dates.datetime2julian(Dates.DateTime(Date(config[1])))
	dimₘᵢₙ::Float64 = 18 + 49 / 60.0
	fov::Int64 = config[2]
	Vₘᵢₙ::Int64 = config[3]
	n_stars::Int64 = config[4]
	#αᵣ = extrema(config[4])
	αᵣ::Tuple{Int64,Int64} = 15 .* config[5:6]
	δᵣ::Tuple{Int64, Int64} = extrema(config[7])
	dimᵣ::Tuple{Float64, Float64} = (fov*dimₘᵢₙ/100, dimₘᵢₙ);
	altₘᵢₙ::Int64 = config[8]
	id_search::String = config[9]
	otype_search::String = config[10]
	hide_missing_vband::Bool = config[11]
	md"###### Config variable bindings"
end

# ╔═╡ ce343675-fcef-4238-a442-dc964e41c843
begin
	const final_fields = [:MAIN_ID, :OTYPE_S, :transitAlt, :FLUX_V, :RA_d, :DEC_d, :GALDIM_MINAXIS];
	const uMissing = Union{Float32, Missing};
	
	function available_stars_2(star_data::DataFrame, excluded_types::Set{String}=Set{String}(); Vₘᵢₙ::Int64=14,αᵣ::Tuple{Int64,Int64}=[0,360], δᵣ::Tuple{Int64,Int64}=[-30,68], dimᵣ::Tuple{Float64,Float64}=[12,18], altₘᵢₙ::Int64=0, n_stars::Int64 = 50, id_search::String = "", otype_search::String= "", hide_missing_vband::Bool = false)

		data = copy(star_data[!, nameₗᵢₛₜ])
		
		original_length = nrow(data)
		data = data[!, Not([
			"OID4"
		])]

		println("Removed OID4: $(original_length - nrow(data))")
		
		# Filter non-matching IDs
		if id_search ≠ ""
			filter!(row -> occursin(id_search, row[:MAIN_ID]), data)
			println("ID Search: $(nrow(data))")
			
			if nrow(data) == 0
				return select!(data, final_fields)
			end
		else
			println("ID Search: SKIPPED")
		end
		
		
		# Filter excluded types
		filter!(row -> row[:OTYPE_S] ∉ excluded_types, data)
		println("OTYPE_S: $(nrow(data))")

		if nrow(data) == 0
			return select!(data, deleteat!(final_fields, 3))
		end

		# Further filter for OTYPE search
		if otype_search ≠ ""
			filter!(row -> occursin(otype_search, row[:OTYPE_S]), data)
			println("OTYPE Search: $(nrow(data))")
			
			if nrow(data) == 0
				return select!(data, deleteat!(final_fields, 3))
			end
		else
			println("OTYPE Search: SKIPPED")
		end

		# Filter rows with missing coordinate data
		original_length = nrow(data)
		filter!(row -> !(ismissing(row[:RA_d]) || ismissing(row[:DEC_d])), data)
		println("Rows missing coordinates: $(original_length - nrow(data))")

		if nrow(data) == 0
			return select!(data, deleteat!(final_fields, 3))
		end

		# Filter rows with missing magnitudes
		original_length = nrow(data)
		filter!(row -> !ismissing(row[:FLUX_V]), data)
		println("Rows missing magnitudes: $(original_length - nrow(data))")

		if nrow(data) == 0
			return select!(data, deleteat!(final_fields, 3))
		end
		

		# Corrected coordinates due to proper motion
		correctedCoords_list::Vector{Matrix{Float64}} = Vector{Vector{Float64}}(undef, nrow(data))

		missing_coords_count::Float64 = 0
		
		@inbounds @simd for i in 1:nrow(data)
			row = data[i,:]
			
			# Correct if proper motion data is present
			potential_missing = (row["PM:pmra"], row["PM:pmde"])

			correctedCoords_list[i] = if !isnothing(findfirst(ismissing, potential_missing))

				[row[:RA_d] row[:DEC_d]]::Matrix{Float64}

			else
				correctCoords(
					curr_JD, row[:RA_d], row[:DEC_d], potential_missing
				)::Matrix{Float64}
			end
		end

		correctedCoords::Matrix{Float64} = reduce(vcat, correctedCoords_list)


		# Correct RA and dec
		data[!, :RA_d] = correctedCoords[:, 1]
		data[!, :DEC_d] = correctedCoords[:, 2]
		
		println("Corrected coordinates for proper motion")
		# data[!, :RA_d] = data[!, :RA_d] ./ 15

		
		# Filter every restriction
		Vₘᵢₙ = -2.5 * log10(n_stars * 10^(-0.4Vₘᵢₙ))
		println("New Vₘᵢₙ: $Vₘᵢₙ")
	
		range_restrictions::Dict{String, Any} = Dict(
			"FLUX_V" => (nothing, Vₘᵢₙ),
			"DEC_d" => δᵣ,
			"GALDIM_MINAXIS" => dimᵣ,
			"GALDIM_MAJAXIS" => dimᵣ,
			"RA_d" => αᵣ
		)
		
		for (colname, restriction) ∈ range_restrictions
			filter!(r -> rbetween(r[colname], restriction), data)
			n_rows::Int = nrow(data)
			println(colname, " ", restriction, " : ", n_rows)

			if n_rows == 0
				return select!(data, deleteat!(final_fields, 3))
			end
		end
		
		
	
		# Calculate transit altitudes
		data[!, :transitAlt] = transitAlt.(data[!, :DEC_d])
		
		# Reduces due to altitudes "above" 90° go back down from 90 (draw hemisphere and label 90deg, upwards)
		filter!(row -> between(row[:transitAlt][1], (altₘᵢₙ, 90)), data)
		println("transitAlt : $(nrow(data))")

		if nrow(data) == 0
			return select!(data, final_fields)
		end
		
	
		# Now calculations are over, append whether star is north or south
		data[!, :transitAlt] = map(data[!, :transitAlt]) do alt
			dms_to_string(deg_to_dms(alt[1])) * (alt[2] ? "North " : "South ")
		end
	
		## Sort data basesd on RA
		# Sort data based on name
		sort!(data, :MAIN_ID)
		
		data[!, :RA_d] = hours_to_string.(deg_to_hours.(data[!, :RA_d]))
		data[!, :DEC_d] = dms_to_string.(deg_to_dms.(data[!, :DEC_d]))
		
		data[!, :MAIN_ID] = replace.(data[!, :MAIN_ID], r"\s+" => " ")
	
		
		# println("Current LST: $(v(curr_JD))")
		# Only show certain fields
		select!(data, final_fields)
		
	end
	md"###### Available Stars"
end

# ╔═╡ 29744835-1abe-4c6d-8af6-52d9678db2cc
if @isdefined dataset
	md"## Transit times may be off by ±10min
(In extreme cases)"
end

# ╔═╡ 21ba0999-2b1f-4c4b-bfaa-e544ea61ae0b
if @isdefined dataset
	excluded_types = Set([ # Aren't visible (Or *very* hard to resolve) in V-Band
			"DarkNeb"
			"HIshell"
			"MolCld"
			# "GalNeb"
			"Bubble"
			"Radio"
			"denseCore"
			"Outflow_Candidate"
			"Galaxy_Candidate"
			"Cluster*_Candidate"
			"Compact_Gr_G" # Too distant/not visible
			"HVCld"
			"HI"
			"Cloud"
			"PartofCloud"
			"smmRad"
			"Infrared"
			"FarIR"
			"RadioG"
			"HIIReg"
			"YSO"
			"PlanetaryNeb"# Infrared
			# "OpenCluster" # Want something interesting!
			#"ClG"
	])
	available_stars_2(dataset, excluded_types; Vₘᵢₙ=Vₘᵢₙ,αᵣ=αᵣ, δᵣ=δᵣ, dimᵣ=dimᵣ, altₘᵢₙ=altₘᵢₙ, n_stars=n_stars, id_search=id_search, otype_search=otype_search,
	hide_missing_vband = hide_missing_vband)
end

# ╔═╡ Cell order:
# ╟─9ca58c70-5bd9-11ee-145e-219a239f1062
# ╟─01d07651-2162-4b9d-b88a-be2002fc309b
# ╟─bc9195bd-9cf8-4f9f-bb7a-cbc4f0359576
# ╟─67da4a5c-9925-42a1-a9f5-b603644567f3
# ╟─032f6250-cd49-462c-9363-0d818e7c8345
# ╟─abb151f6-481b-4bd2-8d06-1eeb21a66a89
# ╟─ce343675-fcef-4238-a442-dc964e41c843
# ╟─9a21c39d-74ad-4c7f-aa40-25a95174a5ef
# ╠═2841f14e-945f-40ef-a33f-404f8adf69b5
# ╟─e0a95451-ea45-4963-b224-bedbbfd1d2a2
# ╟─47329fcd-1cdd-47e2-9dd1-7aeed35b7e28
# ╟─29744835-1abe-4c6d-8af6-52d9678db2cc
# ╟─21ba0999-2b1f-4c4b-bfaa-e544ea61ae0b
