#pragma once

int mqtt_init(void);
void mqtt_send(void);
int mqtt_reconnect(void);
void mqtt_loop(void);
void mqtt_stop(void);