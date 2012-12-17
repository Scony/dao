#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#define NUM_PLAYERS 2

#include <glib-object.h>

G_BEGIN_DECLS

#define TYPE_CONFIGURATION    (configuration_get_type() )
#define CONFIGURATION(obj)    (G_TYPE_CHECK_INSTANCE_CAST( (obj), TYPE_CONFIGURATION, Configuration) )
#define IS_CONFIGURATION(obj) (G_TYPE_CHECK_INSTANCE_TYPE( (obj), TYPE_CONFIGURATION))
#define CONFIGURATION_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST( (klass), TYPE_CONFIGURATION, ConfigurationClass))
#define CONFIGURATION_IS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE( (klass), TYPE_CONFIGURATION))
#define CONFIGURATION_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), CONFIGURATION_TYPE, ConfigurationClass))

typedef struct _PlayerConfiguration PlayerConfiguration;
typedef struct _Configuration Configuration;
typedef struct _ConfigurationClass ConfigurationClass;

enum PlayerType
  {
    PLAYER_HUMAN,
    PLAYER_COMPUTER
  };

enum PlayerAlgorithm
  {
    ALGORITHM_RANDOM,
    ALGORITHM_HILL_CLIMBER
  };

struct _PlayerConfiguration
{
  int color;
  PlayerType type;
  PlayerAlgorithm algorithm;
};

struct _Configuration
{
  GObject parent;

  int first_player;
  PlayerConfiguration players[NUM_PLAYERS];
};

struct _ConfigurationClass
{
  GObjectClass parent_class;
};

GType configuration_get_type(void) G_GNUC_CONST;
Configuration* configuration_new();

G_END_DECLS

#endif
