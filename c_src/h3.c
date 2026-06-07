#include <h3/h3api.h>
#include <erl_nif.h>


static ERL_NIF_TERM
make_h3idx(ErlNifEnv * env, H3Index index)
{
    return enif_make_uint64(env, index);
}

static ERL_NIF_TERM lat_lng_to_cell(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    LatLng latLng;
    int arity;
    const ERL_NIF_TERM *elems;

    int res;
    H3Index out;
	char buf[17];
	

    /* argv[0] = {lat, lng} */
    if (!enif_get_tuple(env, argv[0], &arity, &elems) || arity != 2)
        return enif_make_badarg(env);

    if (!enif_get_double(env, elems[0], &latLng.lat))
        return enif_make_badarg(env);

    if (!enif_get_double(env, elems[1], &latLng.lng))
        return enif_make_badarg(env);

    /* argv[1] = resolution */
    if (!enif_get_int(env, argv[1], &res))
        return enif_make_badarg(env);

    /* call H3 */
    latLng.lat = degsToRads(latLng.lat);
     latLng.lng = degsToRads(latLng.lng);
    H3Error err = latLngToCell(&latLng, res, &out);
	H3Error er = h3ToString(out, buf, sizeof(buf));


    /* return result */
    // return enif_make_uint64(env, out);
    return enif_make_string(env, buf, ERL_NIF_UTF8);
}

static ERL_NIF_TERM cell_to_lat_lng(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]){
	H3Index h3;
	LatLng g;
	char buf[17];

    if(!enif_get_string(env, argv[0], buf, sizeof(buf), ERL_NIF_UTF8))
        return enif_make_badarg(env);
    H3Error errh3 = stringToH3(buf, &h3);
	H3Error err = cellToLatLng(h3, &g);
	
	return enif_make_tuple2(env, enif_make_double(env, radsToDegs(g.lat)),  enif_make_double(env, radsToDegs(g.lng)));

}

static ErlNifFunc nif_funcs[] = {
    {"lat_lng_to_cell", 2, lat_lng_to_cell},
    {"cell_to_lat_lng", 1, cell_to_lat_lng}
};

ERL_NIF_INIT(Elixir.ExH3, nif_funcs, NULL, NULL, NULL, NULL);
