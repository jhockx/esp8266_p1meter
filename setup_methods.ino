void setupOTA()
{
  ArduinoOTA.setPort(8266);

  ArduinoOTA.setHostname(HOSTNAME);
  ArduinoOTA.setPassword(OTA_PASSWORD);

  ArduinoOTA.onError([](ota_error_t error)
  {
    blinkLed(3, 2000); // Blink 3 times slowly to indicate OTA error
  });

  ArduinoOTA.begin();
}

void setupDataReadout()
{
    // 1-0:1.8.1(000992.992*kWh)
    // 1-0:1.8.1 = Elektra verbruik laag tarief (DSMR v5.0)
    telegramObjects[0].name = "consumption_low_tarif";
    strcpy(telegramObjects[0].code, "1-0:1.8.1");
    telegramObjects[0].endChar = '*';

    // 1-0:1.8.2(000560.157*kWh)
    // 1-0:1.8.2 = Elektra verbruik hoog tarief (DSMR v5.0)
    telegramObjects[1].name = "consumption_high_tarif";
    strcpy(telegramObjects[1].code, "1-0:1.8.2");
    telegramObjects[1].endChar = '*';

    // ADDED DURING MERGE ------------------------------------------
    // 1-0:1.8.1(000992.992*kWh)
    // 1-0:1.8.1 =  (DSMR v5.0)
    telegramObjects[2].name = "delivered_low_tarif";
    strcpy(telegramObjects[2].code, "1-0:2.8.1");
    telegramObjects[2].endChar = '*';

    // 1-0:1.8.2(000560.157*kWh)
    // 1-0:1.8.2 =  (DSMR v5.0)
    telegramObjects[3].name = "delivered_high_tarif";
    strcpy(telegramObjects[3].code, "1-0:2.8.2");
    telegramObjects[3].endChar = '*';

    // 0-1:24.2.1(150531200000S)(00811.923*m3)
    // 0-1:24.2.1 = Gas (DSMR v5.0)
    telegramObjects[4].name = "gas_meter_m3";
    strcpy(telegramObjects[4].code, "0-1:24.2.1");
    telegramObjects[4].endChar = '*';
    // END ADDED DURING MERGE ------------------------------------------

    // 1-0:1.7.0(00.424*kW) Actueel verbruik
    // 1-0:1.7.x = Electricity consumption actual usage (DSMR v5.0)
    telegramObjects[5].name = "actual_consumption";
    strcpy(telegramObjects[5].code, "1-0:1.7.0");
    telegramObjects[5].endChar = '*';
    
    // 1-0:2.7.0(00.000*kW) 
    // Actuele teruglevering (-P) in 1 Watt resolution
    telegramObjects[6].name = "actual_received";
    strcpy(telegramObjects[6].code, "1-0:2.7");
    telegramObjects[6].endChar = '*';

    // 1-0:21.7.0(00.378*kW)
    // 1-0:21.7.0 = Instantaan vermogen Elektriciteit levering L1
    telegramObjects[7].name = "instant_power_usage_l1";
    strcpy(telegramObjects[7].code, "1-0:21.7.0");
    telegramObjects[7].endChar = '*';

    // 1-0:41.7.0(00.378*kW)
    // 1-0:41.7.0 = Instantaan vermogen Elektriciteit levering L2
    telegramObjects[8].name = "instant_power_usage_l2";
    strcpy(telegramObjects[8].code, "1-0:41.7.0");
    telegramObjects[8].endChar = '*';

    // 1-0:61.7.0(00.378*kW)
    // 1-0:61.7.0 = Instantaan vermogen Elektriciteit levering L3
    telegramObjects[9].name = "instant_power_usage_l3";
    strcpy(telegramObjects[9].code, "1-0:61.7.0");
    telegramObjects[9].endChar = '*';

    // 1-0:31.7.0(002*A)
    // 1-0:31.7.0 = Instantane stroom Elektriciteit L1
    telegramObjects[10].name = "instant_power_current_l1";
    strcpy(telegramObjects[10].code, "1-0:31.7.0");
    telegramObjects[10].endChar = '*';

    // 1-0:51.7.0(002*A)
    // 1-0:51.7.0 = Instantane stroom Elektriciteit L2
    telegramObjects[11].name = "instant_power_current_l2";
    strcpy(telegramObjects[11].code, "1-0:51.7.0");
    telegramObjects[11].endChar = '*';

    // 1-0:71.7.0(002*A)
    // 1-0:71.7.0 = Instantane stroom Elektriciteit L3
    telegramObjects[12].name = "instant_power_current_l3";
    strcpy(telegramObjects[12].code, "1-0:71.7.0");
    telegramObjects[12].endChar = '*';

    // 1-0:32.7.0(232.0*V)
    // 1-0:32.7.0 = Voltage L1
    telegramObjects[13].name = "instant_voltage_l1";
    strcpy(telegramObjects[13].code, "1-0:32.7.0");
    telegramObjects[13].endChar = '*';

    // 1-0:52.7.0(232.0*V)
    // 1-0:52.7.0 = Voltage L2
    telegramObjects[14].name = "instant_voltage_l2";
    strcpy(telegramObjects[14].code, "1-0:52.7.0");
    telegramObjects[14].endChar = '*';

    // 1-0:72.7.0(232.0*V)
    // 1-0:72.7.0 = Voltage L3
    telegramObjects[15].name = "instant_voltage_l3";
    strcpy(telegramObjects[15].code, "1-0:72.7.0");
    telegramObjects[15].endChar = '*';

    // 0-0:96.14.0(0001)
    // 0-0:96.14.0 = Actual Tarif
    telegramObjects[16].name = "actual_tarif_group";
    strcpy(telegramObjects[16].code, "0-0:96.14.0");

    // 0-0:96.7.21(00003)
    // 0-0:96.7.21 = Aantal onderbrekingen Elektriciteit
    telegramObjects[17].name = "short_power_outages";
    strcpy(telegramObjects[17].code, "0-0:96.7.21");

    // 0-0:96.7.9(00001)
    // 0-0:96.7.9 = Aantal lange onderbrekingen Elektriciteit
    telegramObjects[18].name = "long_power_outages";
    strcpy(telegramObjects[18].code, "0-0:96.7.9");

    // 1-0:32.32.0(00000)
    // 1-0:32.32.0 = Aantal korte spanningsdalingen Elektriciteit in fase 1
    telegramObjects[19].name = "short_power_drops";
    strcpy(telegramObjects[19].code, "1-0:32.32.0");

    // 1-0:32.36.0(00000)
    // 1-0:32.36.0 = Aantal korte spanningsstijgingen Elektriciteit in fase 1
    telegramObjects[20].name = "short_power_peaks";
    strcpy(telegramObjects[20].code, "1-0:32.36.0");
}
