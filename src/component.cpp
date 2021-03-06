#include "component.h"


#include <cstdint>
#include <vector>

std::vector<const char*> component_names;
std::vector<component_t> registered_components;




component_t component_new()
{
  static component_t _default = {NULL,};
  return _default;
}




bool component_register(const char* a_Name, component_t a_Component)
{
  if (!component_exists(a_Name) && component_valid(a_Component))
  {
    component_names.push_back(a_Name);
    registered_components.push_back(a_Component);
    if (a_Component.init_func != NULL)
    {
      a_Component.init_func();
    }
    return true;
  }
  return false;
}




bool component_deregister(const char* a_Name)
{
  size_t count = component_names.size();
  for (size_t i = 0; i < count; ++i)
  {
    if (strcmp(component_names[i], a_Name) == 0)
    {
      component_names[i] = component_names[count - 1];
      component_names.pop_back();
      registered_components[i].cleanup_func();
      registered_components[i] = registered_components[count - 1];
      registered_components.pop_back();
      return true;
    }
  }
  return false;
}




bool component_exists(const char* a_Name)
{
  size_t count = component_names.size();
  for (size_t i = 0; i < count; ++i)
  {
    if (strcmp(component_names[i], a_Name) == 0)
      return true;
  }
  return false;
}




bool component_get(const char* a_Name, component_t* a_Component)
{
  size_t count = component_names.size();
  for (size_t i = 0; i < count; ++i)
  {
    if (strcmp(component_names[i], a_Name) == 0)
    {
      *a_Component = registered_components[i];
      return true;
    }
  }
  return false;
}




bool component_valid(component_t a_Component)
{
  return a_Component.gc_func != NULL && a_Component.cleanup_func != NULL;
}
