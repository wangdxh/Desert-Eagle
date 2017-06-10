#include "stdafx.h"
#include "nice_agent.h"

void cb_nice_recv(NiceAgent *agent, guint stream_id, guint component_id, guint len, gchar *buf, gpointer data)
{   
    nice_agent* pAgent = (nice_agent*)data;
    pAgent->nice_recv_data(stream_id, component_id, len, buf);    
}

void cb_candidate_gathering_done(NiceAgent *agent, guint stream_id, gpointer data)
{
    nice_agent* pAgent = (nice_agent*)data;
    pAgent->candidate_gathering_done(stream_id);
}
void cb_component_state_changed(NiceAgent *agent, guint stream_id, guint component_id, guint state, gpointer data)
{
    nice_agent* pAgent = (nice_agent*)data;
    pAgent->component_state_changed(stream_id, component_id, state);
}

GMainLoop* nice_agent::gloop = NULL;
GThread* nice_agent::gloopthread = NULL;