from django.core.mail import send_mail


def get_client_ip(request):
    x_forwarded_for = request.META.get('HTTP_X_FORWARDED_FOR')
    if x_forwarded_for:
        ip = x_forwarded_for.split(',')[-1].strip()
    elif request.META.get('HTTP_X_REAL_IP'):
        ip = request.META.get('HTTP_X_REAL_IP')
    else:
        ip = request.META.get('REMOTE_ADDR')
    return ip


def inform_admin():
    send_mail(
        'Flower in danger',
        'Flower needs water!',
        'filip.plata@outlook.com',
        ['filip.plata@outlook.com'],
        fail_silently=False,
    )
