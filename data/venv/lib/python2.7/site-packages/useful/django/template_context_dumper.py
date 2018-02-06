import datetime
import os
import bz2

import dill
from django.core.urlresolvers import RegexURLResolver
from django.http.request import HttpRequest
from django.utils.encoding import force_text, force_bytes


#
#                 dump_path = getattr(settings, 'PAGE_DUMP_TEMPLATE_CONTEXT_DIR', None)
#                 if dump_path:
#                     from useful.django.template_context_dumper import dump_template_context
#
#                     dump_template_context(dump_path, template_name, data, context_instance,
#                                           getattr(request, 'urlconf', settings.ROOT_URLCONF))


FIXED_ALLOWED_META = set("""
CONTENT_LENGTH
CONTENT_TYPE
CSRF_COOKIE
DJANGO_SETTINGS_MODULE
GATEWAY_INTERFACE
PATH_INFO
QUERY_STRING
REMOTE_ADDR
REMOTE_HOST
REQUEST_METHOD
SCRIPT_NAME
SERVER_NAME
SERVER_PORT
SERVER_PROTOCOL
SERVER_SOFTWARE
TZ
""".strip().split('\n'))


def dump_template_context(dump_path, template_name, data, context_instance, urlconf):

    dicts_copy = [d.copy() for d in context_instance.dicts]

    for ctx in context_instance:

        if 'messages' in ctx:
            ctx['messages'] = list(ctx['messages'])

        if 'csrf_token' in ctx:
            ctx['csrf_token'] = force_text(ctx['csrf_token'])

        if 'request' in ctx:
            req = ctx['request']
            new_req = HttpRequest()
            for attr in 'GET', 'POST', 'COOKIES', 'path', 'path_info', 'method':
                setattr(new_req, attr, getattr(req, attr))

            new_req.META = dict((k, force_text(v)) for k, v in req.META.items()
                                if (k in FIXED_ALLOWED_META or k.startswith('HTTP_')))

            ctx['request'] = new_req

        if 'sql_queries' in ctx:
            del ctx['sql_queries']

    resolver = RegexURLResolver(r'^/', urlconf)
    # enforce population
    resolver.reverse_dict
    resolver._namespace_dict = {}
    if hasattr(resolver, '_urlconf_module'):
        delattr(resolver, '_urlconf_module')

    for rdict in resolver._reverse_dict.values():
        for key in rdict.keys():
            if callable(key):
                del rdict[key]

    now = datetime.datetime.now()

    out_filename = now.strftime('%Y%m%d-%H%M%S-')
    out_filename += force_bytes(template_name).lower().replace('/', '-SLASH-').replace('.', '-DOT-')
    out_filename += '.template-context-dill.bz2'

    if not os.path.exists(dump_path):
        os.makedirs(dump_path, mode=0755)

    out_filepath = os.path.join(dump_path, out_filename)

    out_file = bz2.BZ2File(out_filepath, 'w')

    try:
        dump = dict(
            now=now,
            template_name=template_name,
            data=data,
            context_instance=context_instance,
            resolver=resolver,
        )

        dill.dump(dump, out_file)
        out_file.close()

    except:
        out_file.close()
        os.unlink(out_filepath)
        raise

    context_instance.dicts = dicts_copy
