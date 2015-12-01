//generate_scaffolding.cpp
//this is algorithm 2 from Bridging the Gap
#include "generate_scaffolding.h"
#include "select_bridge.h"
#include "bridge.h"

//structures needed:
//set_of_active_elements
//set_of_anchoring_segments
//queue_of_events
//set_of_bridges

//functions needed:
//create_anchoring_segments
//intersect_sets
//select_bridge
//difference_sets
//snap_element_and_bridge

//input: a set of points that require supports AND d, the number of sweep directions
//output: a valid bridge structure

//initialize set of active elements with set of points that require supports

/*
while(true)
{
	best_bridge = 0;
	for(i = 0; i < d-1; i++)
	{
		set_of_anchoring_segments = create_anchoring_segments(active_elements, i_direction)
		segments_crossing_sweep_plane = 0;
		queue_of_events = events_from_create_anchoring_segments, sorted by increasing X
		while(queue_of_events is not empty)
		{
			e = pop_queue //leftmost event
			for each anchoring_segment in set_of_anchoring_segments
				starting in e do
				segements_crossing_sweep_plane = segments_crossing_sweep_plane intersected with s
			selected = selectBridge(segments_crossing_sweep_plane)
			if(score_of_selected_bridge > score_of_best_bridge)
			{
				bestBridge = selected;
			}
			for each anchoring_segment in set_of_anchoring_segments
				do segments_crossing_sweep_plane = segments_crossing_sweep_plane less anchoring_segment
		}
	}
	if(best_bridge = 0)
	{
		return;
	}
	C = set of elements supported by bestBridge
	for each c in C
	{
		snap(c, bestBridge)
	}
	active_elements = active_elements unioned with bestBridge
}*/
