#
# Makefile
#
# Makefile for Weather sample
#

include $(POCO_BASE)/build/rules/global

objects = \
	ArrayOfForecast \
	ArrayOfWeatherDescription \
	Forecast \
	ForecastReturn \
	GetCityForecastByZIP \
	GetCityForecastByZIPResponse \
	GetCityWeatherByZIP \
	GetCityWeatherByZIPResponse \
	GetWeatherInformation \
	GetWeatherInformationResponse \
	IWeather \
	POP \
	Temp \
	Weather \
	WeatherClientHelper \
	WeatherDescription \
	WeatherProxy \
	WeatherProxyFactory \
	WeatherReturn \
	WeatherClient

target         = Weather
target_version = 1
target_libs    = PocoRemotingNGSOAP PocoRemotingNG PocoUtil PocoNet PocoXML PocoFoundation

include $(POCO_BASE)/build/rules/exec
