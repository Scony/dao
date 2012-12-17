#include "Configuration.h"

#define CONFIGURATION_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), TYPE_CONFIGURATION, ConfigurationPrivate))

G_DEFINE_TYPE( Configuration, configuration, G_TYPE_OBJECT)

typedef struct _ConfigurationPrivate ConfigurationPrivate;

struct _ConfigurationPrivate
{
  int a;
};

static void configuration_finalize(GObject* configuration);

static void configuration_set_property(GObject* object, 
				       guint prop_id,
				       const GValue *value,
				       GParamSpec *pspec);
static void configuration_get_property(GObject* object,
				       guint prop_id,
				       GValue* value,
				       GParamSpec *pspec);

static void configuration_class_init(ConfigurationClass* klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
  gobject_class->finalize = configuration_finalize;
  gobject_class->get_property = configuration_get_property;
  gobject_class->set_property = configuration_set_property;
}

static void configuration_init(Configuration* configuration)
{
  ConfigurationPrivate* priv = CONFIGURATION_GET_PRIVATE(configuration);
}

static void configuration_finalize(GObject* object)
{
  Configuration* configuration = CONFIGURATION(object);
  ConfigurationPrivate* priv = CONFIGURATION_GET_PRIVATE(configuration);
  GObjectClass* parent_class = G_OBJECT_CLASS(configuration_parent_class);

  (*parent_class->finalize)(object);
}

Configuration* configuration_new()
{
  Configuration* configuration;

  configuration = CONFIGURATION( g_object_new( TYPE_CONFIGURATION, NULL));
  
  return configuration;
}

static void configuration_set_property(GObject* object, 
				       guint prop_id,
				       const GValue *value,
				       GParamSpec *pspec)
{
  G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec);
}

static void configuration_get_property(GObject* object,
				       guint prop_id,
				       GValue* value,
				       GParamSpec *pspec)
{
  G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec);
}
